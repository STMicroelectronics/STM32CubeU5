Release Cadence and Process
===========================

Project Release Cadence
-----------------------

The project currently aims to do a release once every 4 months which will be
tagged on the master branch. There will be a code freeze (stop merging
non-essential changes) up to 3 weeks prior to the target release date. The
release candidates will start appearing after this and only bug fixes or
updates required for the release will be merged. The maintainers are free
to use their judgement on what changes are essential for the release.

The release testing will be performed on release candidates and depending on
issues found, additional release candidates may be created to fix the issues.

::

                             |<------------4 months----------->|
          |<--upto 3 weeks-->|              |<--upto 3 weeks-->|
    +----------------------------------------------------- ----------> time
          |                  |              |                  |
        code freeze        ver w.x        code freeze        ver y.z

Although this document specifies the release cadence, this does not preclude
an adhoc release for specific project requirements.

Release Version Scheme
----------------------

Trusted Firmware-M uses a semantic versioning scheme. A version number is
compiled as a dot separated set of numbers:

**TF-Mv<MAJOR>.<MINOR>.<HOTFIX>**

- <MAJOR>: Major release version for significant feature and API changes.
- <MINOR>: Minor release version for incremental features and API changes.
- <HOTFIX>: Used only for backporting **critical bug fix/security patches**.

--------------

*Copyright (c) 2020, Arm Limited. All rights reserved.*
