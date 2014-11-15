Nagios-Python-Broker
====================

Provides an *Event Broker* for **Nagios** with a Python API.

Technical Description
---------------------

Using the C API of Python, all data received in the broker is translated to a
Python Object, and will be transmitted to the event handler implemented in Python.

How to build
------------

.. code-block:: bash

   ./bootsrap
   ./configure
   make

The **Nagios** module will be available at ``.libs/libnagios-python-broker.so``.

Configuration
-------------

Add this line to your ``nagios.cfg`` :

.. code-block:: ini

   broker_module = /path/to/libnagios-python-broker.so mypython.package

Make sure ``event_broker_options`` is set to ``-1``.
