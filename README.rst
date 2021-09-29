******************************************************
ICSFuzz: Fuzzing Tool for ICS Control Applicatons
******************************************************

Overview
========

``ICSFuzz`` is an PLC-side fuzzing tool for uncovering vulnerabilities in ICS control applications

by Dimitris Tychalas `\@ditihala`_

.. _`\@ditihala`: https://www.twitter.com/ditihala




Installation
============

The tool requires an already existing cross compiler on your machine. Since the fuzzer runs natively on the PLC, it needs to be compiled with an ARM-based cross-toolchain, such as OSELAS. For installing such a toolchain please follow the instructions on the following link and modify the Makefile with the location of your cross-compiler.

* `OSELAS toolchain <https://pengutronix.de/en/software/toolchain.html>`__


Getting Started
=============

The ``ICSFuzz`` tool is a specialized security assessment tool for evaluating ICS control applications. At this current stage, it supports only applications based on the ``Codesys`` platform which has been modified and adapted for the ``Wago`` PLC. While this tool is an ongoing effort on our side, any suggestions for upgrades and enhanced compatibility are more than welcome :) Just shoot me an email at dimitris.tychalas@nyu.edu

* This tool is build as a simple application that will be run on your Wago PLC. Just run the Makefile, copy the produced ``fuzzer`` binary on your PLC and execute it! Since the tool is accessing and modifying arbitrary process memory, it requires admin privileges. Please make sure to execute it as a sudo or root user.

* The tool is currently compatible with Codesys 3.5, patch 02.06.20(08) and older versions, please upgrade or downgrade your PLC firmware so it can host ICSFuzz properly. For visual feedback on the fuzzing process, you may use the e!cockpit platform which is offered as a development/HMI tool for Wago PLC. Through it, you can track the fuzzing input delivered to the application as well as get informed on a potential crash, as the application will stop executing, the "run" LED will get stuck on red. Since the application will stop, the current (stuck) input is the one that caused the crash. You may restart the application through the e!cockpit and restart the fuzzer once more.

* The fuzzer has an initial hard-coded starting value (note as seed_input in the source code) which you may modify at will as you play around with the fuzzer.


Cite us!
========
If you find our work interesting and use it in your (academic or not) research, please cite our Usenix Security 2021 paper describing ICSFuzz:

Tychalas, Dimitrios, Hadjer Benkraouda, and Michail Maniatakos. "ICSFuzz: Manipulating I/Os and Repurposing Binary Code to Enable Instrumented Fuzzing in {ICS} Control Applications." 30th {USENIX} Security Symposium ({USENIX} Security 21). 2021.

Acknowledgements
================

``ICSFuzz``, as all things good in life, is based on the shoulder of giants. The framework is based on the powerful ``AFL`` by Michal Zalewski for producing the necessary input mutations that are delivered to the ICS application.

* `AFL <https://lcamtuf.coredump.cx/afl/>`__
