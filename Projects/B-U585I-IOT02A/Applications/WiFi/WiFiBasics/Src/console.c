/**
  ******************************************************************************
  * @file      console.c
  * @author    MCD Application Team
  * @brief     This file provides code for console application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "console.h"
#include "net_connect.h"
#include <inttypes.h>

int32_t quit_msg = 0;


#define MAX_ARGS         10
#define MAX_INPUT_LINE   80

#define KEY_UP          (char) 128
#define KEY_DOWN        (char) 129
#define KEY_LEFT        (char) 130
#define KEY_RIGHT       (char) 131
#define KEY_BACKSPACE            8
#define KEY_DELETE_MODE        127
#define KEY_INSERT_MODE        126


#define TERM_LINE_ERASE                "\r\x1b[2K"
#define TERM_NO_LOCAL_ECHO             "\x1b[12h"
#define TERM_BLINKING_CURSOR           "\x1b[?12h"
#define TERM_NO_BLINKING_CURSOR        "\x1b[?12l"

#define TERM_CURSOR_LEFT               "\x1b[1D"
#define TERM_CURSOR_RIGHT              "\x1b[1C"

#define HISTORY_SIZE       5
#define MAX_MATCHED_CMD    20

/* #define gprompt "NwLib>" */
static const char *gprompt;
static char *history_cmd[HISTORY_SIZE];
static uint32_t used_cmd_count = 0;
static cmd_t *cmd_match[MAX_MATCHED_CMD];
static int32_t insert_mode = 1;

static cmd_t **cmd_match_list(cmd_t cmdlist[], char *name, int32_t *match_count);
static cmd_t *console_cmd_match(cmd_t cmdlist[], char *cmdname);
static void history_free_list(void);
static void history_cmd_list(void);
static void history_cmd_add(char *cmd);
static char local_getc(void);
static char mygetc(void);
static void mygets(char *s, cmd_t cmdlist[]);
static void getcmd(char *s, cmd_t cmdlist[]);
static char *mystrdup(char *s);



int32_t help_cmd(int32_t argc, char **argv)
{
  UNUSED(argc);
  UNUSED(argv);
  extern cmd_t cmdlist[];
  uint32_t i = 0;
  do
  {
    printf("\"%10s\"            %s\n", cmdlist[i].name, cmdlist[i].comment);
    i = i + 1;
  } while (cmdlist[i].name != NULL);
  return 0;
}

int32_t quit_cmd(int32_t argc, char **argv)
{
  UNUSED(argc);
  UNUSED(argv);
  quit_msg = 1;
  return 0;
}

static char *mystrdup(char *s)
{
  char *p = NULL;

  if (s != NULL)
  {
    const size_t n = strlen(s) + 1;
    p = malloc(n);
    if (p != NULL)
    {
      memcpy(p, s, n);
    }
  }
  return p ;
}

static cmd_t **cmd_match_list(cmd_t cmdlist[], char *name, int32_t *match_count)
{
  size_t n = strlen(name);

  *match_count = 0;

  if (n == 0)
  {
    return NULL;
  }
  if (cmdlist == NULL)
  {
    return NULL;
  }
  while (cmdlist->name)
  {
    if (strncmp(cmdlist->name, name, n) == 0)
    {
      /* perfect match */
      if (strlen(cmdlist->name) == n)
      {
        *match_count = 1;
        cmd_match[0] = cmdlist;
        return cmd_match;
      }
      cmd_match[*match_count] = cmdlist;
      *match_count = *match_count + 1;
    }
    cmdlist++;
  }
  if (*match_count > 0)
  {
    return cmd_match;
  }
  return NULL;
}

static cmd_t *console_cmd_match(cmd_t cmdlist[], char *cmdname)
{
  cmd_t **cmd;
  int32_t match_count = 0;

  cmd = cmd_match_list(cmdlist, cmdname, &match_count);
  if (1 == match_count)
  {
    return *cmd;
  }
  /* Ambiguous or no match found */
  return NULL;
}


static void history_free_list(void)
{
  for (uint32_t i = 0; i < HISTORY_SIZE; i++)
  {
    if (history_cmd[i] != NULL)
    {
      free(history_cmd[i]);
      history_cmd[i] = NULL;
    }
  }
}

static void history_cmd_list(void)
{
  for (uint32_t i = 0; i < used_cmd_count; i++)
  {
    printf("%"PRIu32"\t%s\n", i, history_cmd[i]);
  }
  printf("%s", gprompt);
}

static void history_cmd_add(char *cmd)
{
  uint32_t i;
  if (used_cmd_count == 0)
  {
    memset(history_cmd, 0, sizeof(history_cmd));
  }
  for (i = 0; i < HISTORY_SIZE; i++)
  {
    if ((history_cmd[i] != NULL) && (strcmp(history_cmd[i], cmd) == 0))
    {
      if (i == 0)
      {
        return;
      }

      /* we got a match so we should reorder the array. */
      char *localcmd = history_cmd[i];
      for (; i > 0; i--)
      {
        history_cmd[i] = history_cmd[i - 1];
      }
      history_cmd[0] = localcmd;
      return;
    }

    if (history_cmd[i] == NULL)
    {
      for (; i > 0; i--)
      {
        history_cmd[i] = history_cmd[i - 1];
      }
      history_cmd[0] = mystrdup(cmd);
      used_cmd_count++;
      return;
    }
  }
  /* not found and no place , so erase oldest command in list */
  free(history_cmd[HISTORY_SIZE - 1]);
  for (i = HISTORY_SIZE - 1; i > 0; i--)
  {
    history_cmd[i] = history_cmd[i - 1];
  }
  history_cmd[0] = mystrdup(cmd);
}

static char local_getc(void)
{
  int c;
  c = getc(stdin);

  if (EOF != c)
  {
    return (char)c;
  }
  else
  {
    return '\0';
  }
}

static char mygetc(void)
{
  char c;
  char d;

  c = local_getc();
  if (c == 27)
  {
    c = local_getc();
    d = local_getc();

    if ((c == 91) && (d == 50))
    {
      /* discard 0x7E */
      (void)local_getc();
      return KEY_INSERT_MODE;
    }
    /* Numeric Pad */
    if ((c == 79) && (d == 83))
    {
      return '-';
    }
    if ((c == 79) && (d == 82))
    {
      return '*';
    }
    if ((c == 79) && (d == 81))
    {
      return '/';
    }
    /* Left Right Arrows */

    /*  UP ARROW */
    if ((c == 91) && (d == 65))
    {
      return KEY_UP;
    }
    if ((c == 91) && (d == 66))
    {
      return KEY_DOWN;
    }
    if ((c == 91) && (d == 67))
    {
      return KEY_RIGHT;
    }
    if ((c == 91) && (d == 68))
    {
      return KEY_LEFT;
    }
  }

  return c;
}

static void mygets(char *s, cmd_t cmdlist[])
{
  size_t i = 0;
  uint32_t cursor_x = 0;
  char ch = 0;
  int32_t  history_count = -1;
  s[0] = 0;

  do
  {
    ch = mygetc();
    if (ch == KEY_DELETE_MODE)
    {
      ch = KEY_BACKSPACE;
    }
    if (ch == KEY_INSERT_MODE)
    {
      insert_mode = 1 - insert_mode;
      if (insert_mode)
      {
        /* blinking cursor */
        printf(TERM_BLINKING_CURSOR);
      }
      else
      {
        printf(TERM_NO_BLINKING_CURSOR);
      }
    }
    else if (ch == KEY_UP)
    {
      if (history_count < (HISTORY_SIZE - 1))
      {
        history_count++;
      }

      if (NULL != history_cmd[history_count])
      {
        printf(TERM_LINE_ERASE);
        printf("%s", gprompt);
        printf("%s", history_cmd[history_count]);
        i = strlen(history_cmd[history_count]);
        strcpy(s, history_cmd[history_count]);
        cursor_x = i;
      }
    }
    else if (ch == KEY_DOWN)
    {
      if (history_count > 0)
      {
        history_count--;
      }
      if (history_count < 0)
      {
        history_count = 0;
      }

      if (NULL != history_cmd[history_count])
      {
        printf(TERM_LINE_ERASE);
        printf("%s", gprompt);
        printf("%s", history_cmd[history_count]);
        i = strlen(history_cmd[history_count]);
        cursor_x = i;
        strcpy(s, history_cmd[history_count]);
      }
    }
    else if (ch == KEY_LEFT)
    {
      if (cursor_x > 0)
      {
        printf(TERM_CURSOR_LEFT);
        cursor_x--;
      }
    }
    else if (ch == KEY_RIGHT)
    {
      if (cursor_x < i)
      {
        cursor_x++;
        printf(TERM_CURSOR_RIGHT);
      }
    }
    else if (ch == KEY_BACKSPACE)  /* Back space or Suppr  */
    {
      if (cursor_x > 0)
      {
        memmove(&s[cursor_x - 1], &s[cursor_x], 1 + i - cursor_x);
        s[i] = 0;
        printf(TERM_LINE_ERASE);
        printf("%s", gprompt);
        printf("%s", s);
        cursor_x--;
        i--;
        /* replace cursor , moving it on the left */
        if (cursor_x < i)
        {
          printf("\x1b[%"PRIu32"D", i - cursor_x);
        }
      }
    }
    else if (ch == '\t')  /* Tabulation */
    {
      cmd_t **match_list;
      cmd_t *cmd;
      int32_t match = 0;

      s[i] = 0;
      match_list = cmd_match_list(cmdlist, s, &match);
      if (match == 1)
      {
        cmd = match_list[0];
        strcpy(s, cmd->name);
        printf(TERM_LINE_ERASE);
        printf("%s", gprompt);
        printf("%s", s);
        i = cursor_x = strlen(s);
      }
      else if (match > 1)
      {
        printf("\n");
        for (int32_t j = 0; j < match ; j++)
        {
          cmd = match_list[j];
          printf("%s\n", cmd->name);
        }
        printf(TERM_LINE_ERASE);
        printf("%s", gprompt);
        printf("%s", s);
        i = cursor_x = strlen(s);
      }
    }
    else
    {
      if (ch != '\n')
      {
        if (insert_mode & (cursor_x != i))
        {
          /* shift right the string from cursor */
          for (uint32_t j = i; j >= cursor_x; j--)
          {
            s[j + 1] = s[j];
          }
          /* add the new char */
          s[cursor_x++] = ch;
          i++;
          printf(TERM_LINE_ERASE);
          printf("%s", gprompt);
          printf("%s", s);
          /* replace cursor, moving it on the left */
          if (cursor_x < i)
          {
            printf("\x1b[%"PRIu32"D", i - cursor_x);
          }
        }
        else
        {
          s[cursor_x++] = ch;
          printf("%c", ch);
          if (cursor_x > i)
          {
            i = cursor_x;
          }
        }
      }

    }
    if (i == MAX_INPUT_LINE)
    {
      i = MAX_INPUT_LINE - 1;
      break;
    }
  } while (ch != '\n');
  s[i] = 0;
  printf("\n");
}

static void getcmd(char *s, cmd_t cmdlist[])
{
  while (1)
  {
    mygets(s, cmdlist);
    if (s[0] == '!')
    {
      int32_t n = atoi(&s[1]);
      if ((n >= 0) && (((uint32_t)n) <= used_cmd_count))
      {
        strcpy(s, history_cmd[n]);
        return;
      }
    }
    else
    {
      if (strcmp(s, "history") == 0)
      {
        history_cmd_list();
      }
      else
      {
        return;
      }
    }
  }
}

void console(const char *prompt, cmd_t cmdlist[])
{
  char s[MAX_INPUT_LINE];
  char scopy[MAX_INPUT_LINE];
  char *args[MAX_ARGS];

  setbuf(stdout, NULL);
  gprompt = prompt;
  printf(TERM_NO_LOCAL_ECHO);

  do
  {
    cmd_t *cmd;
    int32_t i = 0;

    printf("%s", gprompt);
    getcmd(s, cmdlist);

    if (s[0] != 0)
    {
      strcpy(scopy, s);
      args[i] = strtok(s, " \n\t");

      cmd = console_cmd_match(cmdlist, args[i]);
      if (NULL != cmd)
      {
        while (args[i])
        {
          i++;
          args[i] = strtok(NULL, " \n\t");
          if (i == MAX_ARGS)
          {
            break;
          }
        }
        cmd->func(i, args);
        history_cmd_add(scopy);
      }
      else
      {
        printf("console:%s: command not found\n", scopy);
      }
    }
  } while (quit_msg == 0);
  history_free_list();
}
