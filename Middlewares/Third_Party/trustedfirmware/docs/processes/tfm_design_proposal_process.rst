Design proposal process
=======================

:Author: Gyorgy Szing
:Organisation: Arm Limited
:Contact: Gyorgy Szing <gyorgy.szing@arm.com>

Purpose and Content
-------------------
This document describes the steps of changing Trusted Firmware design. It
specifies:

  - the documentation format to be used
  - the information which shall be captured
  - the steps of the process
  - and the location where the information shall be captured during the process.

General
-------
The Trusted Firmware project uses the
`reStructuredText <http://docutils.sourceforge.net/rst.html>`_ format with
`Sphinx <http://www.sphinx-doc.org/en/master/usage/restructuredtext/index.html>`_
extensions for documentation. Design documents shall be captured in this format.

Design documents are kept under version control at the project's
`Gerrit server <https://review.trustedfirmware.org>`_. All decisions made and
important information gathered during the design discussion, which is  not part
of the design document shall be captured as Gerrit comments or notes for
archiving purposes. To meet this requirement this process encourages the use of
the Gerrit web UI for communication.


Status of a document
---------------------
The status of the document is captured in a *reST filed* called *Status*.
Bibliographic fields like the *Status* shall be kept near to the top of the
document after the document title.

Example document fragment::

    TF-M Crypto Service design
    --------------------------

    :Author: Antonio de Angelis
    :Organization: Arm Limited
    :Contact: Antonio de Angelis <antonio.deangelis@arm.com>
    :Status: Draft

Design documents are kept in three different sections of the documentation
reflecting the status of the document. The status of the document determines
the section it is in. Open (*Draft* and *Detailed* status) and accepted design
documents shall be put to the ``docs/design_documents``
directory and rejected documents to the ``docs/design_documents/rejected``
directory.

Preparation
-------------
In order to work on TF-M documentation the TF-M git repository has to be
available locally. Setting up the documentation tools will allow pre-viewing the
documentation file in preparation.
For information on what tools are needed please refer to
:doc:`sw requirements </docs/user_guides/tfm_sw_requirement>`. To see how to get
a local copy of the TF-M repository please see
:doc:`build instructions </docs/user_guides/tfm_build_instruction>`

Process steps
-------------

- Write the design proposal in the format that is described in this document
  with the status set to *Draft*. Put it to the ``docs/design_documents``
  directory and create a pull request.
- Start an e-mail thread on the
  `TF-M mailing list <mailto:tf-m@lists.trustedfirmware.org>`_ for discussing
  the proposal.
- Build initial consensus within the community about the proposed design
  change, rework it according to the feedbacks and identify members who would
  like to participate in the detailed review.
- When the "short list" of members who are willing to participate in the
  detailed review is established, set the *Status* field to *Detailed* and
  push the change to Gerrit.
- Add the members of the "short list" to the Gerrit review as reviewers.
- The detailed discussion then takes place in the Gerrit review and gets
  recorded there.
  Additional changes are submitted as new commits to the review.
- When the proposal is accepted, the status field is set to *Accepted*, the
  change updated and merged.
- If at any point the proposal is rejected its status field is set to
  *Rejected*, and the document is moved to the *rejected design documents*
  section.

.. uml::

  @startuml
  !define DRAFT_DIR **docs/design_documents/**
  !define REJECTED_DIR **docs/design_documents/rejected/**
  !define GERRIT_URL https://review.trustedfirmware.org
  !define GERRIT_LINK [[GERRIT_URL trustedfirmware.org]]
  !define MAINTAINER_RST_URL https://git.trustedfirmware.org/trusted-firmware-m.git/tree/docs/maintainers.rst
  !define TFM_MAILING_LIST mailto:tf-m@lists.trustedfirmware.org
  !define NO_DECISION **no**
  !define YES_DECISION **yes**
  !define STATUS_DRAFT **Draft**
  !define STATUS_DETAILED **Detailed**
  !define STATUS_REJECTED **Rejected**
  !define STATUS_ACCEPTED **Accepted**

  title Design Proposal Process

  start
  :Create first draft.in [[http://docutils.sourceforge.net/rst.html ReST format]];
  :Set it's status to STATUS_DRAFT.;

  :Add your document under DRAFT_DIR.;
  :Create pull-request at GERRIT_LINK.;
  partition "Initial review." {
      :Start an e-mail thread at [[TFM_MAILING_LIST tf-m mailing list]].;
      repeat
          :Build initial consensus within the
           community about the proposed design change.;
          :Gather developers interested in detailed review.;
       repeat while (Ready for detailed review?)
  }

  partition "Detailed review." {
      :Set document status to STATUS_DETAILED.;
      :Add reviewers to pull request.;

      repeat
         :Discuss design in Gerrit comments/notes.;
         :Log the result of discussions over
          other communication channels
          as Gerrit comments/notes.;
         :Push new document version if needed.;
      repeat while (Consensus reached?)
  }

  if (Design is rejected?) then (YES_DECISION)
      :Set document status to STATUS_REJECTED.;
      :Move the document to REJECTED_DIR.;
  else (NO_DECISION)
      :Sets the document status to STATUS_ACCEPTED.;
  endif

  ://Submit// the pull-request.;
  stop

  @enduml

--------------

*Copyright (c) 2019, Arm Limited. All rights reserved.*
