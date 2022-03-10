:Page authors: Gyorgy Szing <gyorgy.szing@arm.com>

..
   The build-system will copy all documents into a temporary directory tree
   before the documentation is built.
   This file will be copied to the top level and thus please use relative paths
   as if this file would be in <TFM_ROOT_DIR>.

   The values between @ characters will be filled in by CMake.

Trusted Firmware-M Documentation
================================

.. raw:: html

    <ul class="grid">
        <li class="grid-item">
            <a href="docs/introduction/readme.html">
                <img alt="" src="_static/images/tfm.png"/>
                <h2>Introduction</h2>
            </a>
            <p>Introducing the Trusted Firmware-M Project: overview,
               architecture, features, and licensing</p>
        </li>
        <li class="grid-item">
            <a href="docs/getting_started/index.html">
               <img alt="" src="_static/images/tfm.png"/>
               <h2>Getting Started Guides</h2>
            </a>
            <p>Follow this guide to set up a development environment on your
               system, and then build and run a sample application.</p>
        </li>
        <li class="grid-item">
            <a href="docs/contributing/index.html">
                <img alt="" src="_static/images/tfm.png"/>
                <h2>Contribution Guidelines</h2>
            </a>
            <p>As an open-source project, we welcome and encourage the community
               to submit patches directly to the project.</p>
        </li>
        <li class="grid-item">
            <a href="docs/reference/index.html">
                <img alt="" src="_static/images/tfm.png"/>
                <h2>References</h2>
            </a>
            <p>User guides, API Documentation, interfaces</p>
        </li>
        <li class="grid-item">
            <a href="docs/design_documents/index.html">
                <img alt="" src="_static/images/tfm.png"/>
                <h2>Design</h2>
            </a>
            <p>Design documents, threat models.</p>
        </li>
        <li class="grid-item">
            <a href="platform/ext/index.html">
                <img alt="" src="_static/images/tfm.png"/>
                <h2>Platforms</h2>
            </a>
            <p>List of supported boards and platforms.</p>
        </li>
        <li class="grid-item">
            <a href="https://ci.trustedfirmware.org/view/TF-M/job/tf-m-build-docs-nightly/lastSuccessfulBuild/artifact/trusted-firmware-m/build/docs/reference_manual/html/index.html">
                <img alt="" src="_static/images/tfm.png"/>
                <h2>API</h2>
            </a>
            <p>Doxygen documentation.</p>
        </li>
        <li class="grid-item">
        <a href="https://developer.arm.com/architectures/security-architectures/platform-security-architecture">
            <img alt="" src="_static/images/tfm.png"/>
            <h2>PSA</h2>
        </a>
        <p>Platform Security Architecture (PSA) information.</p>
        </li>
    </ul>

.. toctree::
    :caption: Table of Contents
    :name: mastertoc
    :titlesonly:
    :maxdepth: 1
    :hidden:

    Home<self>
    docs/introduction/index
    docs/getting_started/index
    docs/contributing/index
    docs/reference/index
    docs/design_documents/index
    platform/ext/index
    docs/threat_models/index


.. toctree::
    :maxdepth: 2
    :hidden:
    :caption: Quick Links

    Security Center <https://developer.trustedfirmware.org/w/collaboration/security_center>
    docs/reference/changelog
    docs/reference/glossary
    docs/contributing/lic

--------------

*Copyright (c) 2017-2020, Arm Limited. All rights reserved.*
