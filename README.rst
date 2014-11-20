Nagios-Python-Broker
====================

Provides an *Event Broker* for **Nagios**/**Icinga** with a Python API.

.. image:: https://travis-ci.org/linkdd/nagios-python-broker.svg?branch=develop
    :target: https://travis-ci.org/linkdd/nagios-python-broker

Technical Description
---------------------

Using the C API of Python, all data received in the broker is translated to a
Python Object, and will be transmitted to the event handler implemented in Python.

How to install
--------------

.. code-block:: bash

   ./bootsrap
   ./configure --prefix=/usr/local/nagios
   make
   make install

The **Nagios** module will be available at ``/usr/local/nagios/lib/libnagios-python-broker.so``.

Configuration
-------------

Add this line to your ``nagios.cfg`` :

.. code-block:: ini

   broker_module = /usr/local/nagios/lib/libnagios-python-broker.so mypython.package

Make sure ``event_broker_options`` is set to ``-1``.
