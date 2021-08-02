:orphan:

Measuring Time Along Packet Flows
=================================

Goals
-----

This showcase demonstrates measuring time associated with a packet as it travels/when it's present in a network, such as total elapsed time, time spent in queues, or transmission time. 

This showcase demonstrates timing measurments along packet flows, i.e. paths various packets take in the network.

The Model
---------

Overview of Packet Flows
~~~~~~~~~~~~~~~~~~~~~~~~

..     so

    - the default statistic collection in inet is based on modules
    - the modules can only collect data they have access to
    - for more versatile timing measurements, packet flows can be defined
    - what measurements its good for
    - what are packet flows
    - how are they created

.. By default, statistics in INET are collected by modules, based on data the modules can access. For more complex timing measurements that require access to data of multiple modules, `packet flows` can be defined.

.. For example, a TCP module communicating with multiple other TCP modules can't distinguish between the packets based on the path they took.

By default, statistics in INET are collected by modules, only based on data the modules can access. Sometimes this might not be sufficient. For example, measuring time separately for packets that arrived at the same destination but took different paths in the network.

.. For example, a TCP sink module communicating with multiple other TCP modules can't distinguish between the packets based on the path they took.

.. **TODO** example where totally obvious that it cant be done without packet flows

.. - nem derul ki h egy csomag milyen utvonalon kozlekedett; timing measurements for different paths - kulon statisztikailag merni a kulonbozo utvonalakat

For timing measurements that require access to data of multiple modules, `packet flows` can be defined. A packet flow is a logical classification of packets, identified by its name, in the whole network and over the whole duration of the simulation. Flows are defined by active modules which create entry and exit points of the flow in the network. The following are some more properties of packet flows:

- A packet can be part of any number of flows
- A packet can enter and exit a flow multiple times
- Flows can overlap in time and in network topology
- Flows can have multiple entry and exit points

.. so

  - packets enter the flow (can be filtering)
  - define the flow name and a filter and measurements
  - measurements are added on the way by modules in the network as metadata
  - packet exit the flow and the statistics are collected

  so

  - packets enter the flow by being tagged by active modules/the module

  active modules tag certain packets, so that they enter the flow (get a FlowTag or something)
  it tags certain packets, that is those matching a filter, for example
  similarly, the X module does the same for packets exiting the flow

  the modules have parameters, like the name of the flow

  what is the process like? actually

  - active modules can be inserted into the path of packets in the network, anywhere in the network
  - these modules classify packets to be entering the flow, e.g. packets matching a filter
  - it adds a tag
  - measurements are added on the way by modules in the network as metadata/tags
  - the packets exit the flow at module X, and statistics of the flow are collected there

.. The FlowMeasurementStarter module serves as the flow entry point for packets. It classifies certain packets (those matching a filter) to be entering the flow,
   by adding a FlowTag to the packet. TODO Also, flow name, required measurements (packet/packetData filter)

.. The FlowMeasurementStarter module creates a flow according to the name parameter, which identifies the flow.

.. **TODO** The MeasurementLayer module; locations (interfaces)(LayeredEthernetInterface currently)

.. **TODO** example for inserting measurement modules in the network at various locations (inside network nodes at arbitrary places, interfaces)

.. The module has parameters for flow name, the required measurements, and packet filter/packet data filter. 

.. so

  - overview: the flow...certain packets enter the flow at active modules (by matching a filter)
  - the active modules add a TAG that describes the kind of measurements requested
  - based on the tag, the measurements are added to the packet as metadata by certain modules as it travels in the network
  - for example, we specify queueing time to be measured. The queues along the packet's route add the measured queueing time to the packet's metadata
  - packets exit the flow at modules acting as exit points. The measurements are recorded there as statistics.(as that module's statistics)(containing the total queueing time incurred by the packet

Certain packets are classified to be entering the flow by FlowMeasurementStarter modules, for example by matching a filter. The modules add a FlowTag that describes the kind of measurements requested by the user. Based on the FlowTag, the measurements are added to the packet as metadata by certain modules as it travels in the network.

For example, we specify queueing time to be measured. The queues along the packet's route add the measured queueing time to the packet's metadata.
Packets exit the flow at FlowMeasurementRecorder modules. The measurements are recorded there as the FlowMeasurementRecorder module's statistics, containing the total queueing time incurred by the packet.

.. **TODO** the details of FlowMeasurementStarter and FlowMeasurementRecorder; it can be put anywhere in the packets' path; measurementLayer; limitations

.. **V1** The FlowMeasurementStarter and FlowMeasurementRecorder modules have the same set of parameters. The main parameters are the following:

  - :par:`flowName`: Identifies the flow the packets are entering or exiting.
  - :par:`measure`: Specifies the requested measurements. The values can be one or the combination of the following:
    
    - ``elapsedTime``: The total elapsed time for the packet being in the flow
    - ``delayingTime``, ``queueingTime``, ``processingTime``, ``transmissionTime``, ``propagationTime``: Total time for different kinds/cases
    - ``packetEvent``: Record all events that happen to the packet (more details below)
    
  - :par:`packetFilter` and :par:`packetDataFilter`: to filter which packets enter or exit the flown access. Sometimes this might not be sufficient. For example, a TCP module communicating with multiple other TCP modules can’t distinguish between the packets based on the path they took.

  For more complex timing measurements that require access to data of multiple modules, packet flows can be defined. A packet flow is a logical classification of packets, identified by its name, in the whole network and over the whole duration of the simulation. Flows are defined by active modules which create entry and exit points of the flow in the network. The following are some more properties of packet flows:

    - :par:`measure`: Specifies the requested measurements. The values can be one or the combination of the following:
      
      - ``elapsedTime``: the total elapsed time for the packet being in the flow
      - ``delayingTime``, ``queueingTime``, ``processingTime``, ``transmissionTime``, ``propagationTime``: the packet spends various times in these categories
      - ``packetEvent``: Record all events that happen to the packet (more details below)

  The ``packetEvent`` measurement is special, in the sense that it doesn't record certain durations as the other measurements. It adds to the packet as metadata all the events that happen to the packet, i.e. those measured by the other measurements. For example, it records events of the packet's delaying, queueing, processing, transmission, etc. **TODO** the user needs to create a module to extract and use this data

The FlowMeasurementStarter and FlowMeasurementRecorder modules have the same set of parameters that specify the flow name (:par:`flowName` parameter), the set of packets that enter or exit the flow (:par:`packetFilter` and :par:`packetDataFilter` parameters), and the required measurements (:par:`measure` parameter).

By default, the filters match all packets (``*``). The :par:`measure` parameter can be one or the combination  of the following:

.. - The total elapsed time for the packet being in the flow: ``elapsedTime``
   - Time for the different cases: ``delayingTime``, ``queueingTime``, ``processingTime``, ``transmissionTime``, ``propagationTime``
   - Record all events that happen to the packet (more details below): ``packetEvent``

- ``elapsedTime``: The total elapsed time for the packet being in the flow
- ``delayingTime``, ``queueingTime``, ``processingTime``, ``transmissionTime``, ``propagationTime``: Time for the different cases
- ``packetEvent``: Record all events that happen to the packet (more details below)

The ``packetEvent`` measurement is special, in the sense that it doesn't record certain durations as the other measurements. It adds to the packet as metadata all the events that happen to the packet. For example, it records events of the packet's delaying, queueing, processing, transmission, etc. Currently, no module makes use of the data added by the ``packetEvent`` measurement; the user needs to create a module to extract and use this data. 

.. **TODO** the user needs to create a module to extract and use this data

   Currently, no module records this data; the user needs to create a module to extract and use this data

   Currently, no module makes use of this feature; the user needs to create a module to extract and use this data. 

.. **TODO** the measurements do they need to be the same for the entry and exit modules? whats do they mean for each of them? for example, in entry module, specify measurements to attach to packet; in exit module, specify which measurements to record?

.. note:: Although both the entry and exit module has a :par:`measure` parameter, its meaning is slightly different for the two modules. For the entry module, the parameter specifies which measurement data to include in the attached flow tag. For the exit module, it specifies which measurements to record as statistics. Generally, the parameter values should be the same for both modules.

.. The FlowMeasurementStarter and FlowMeasurementRecorder modules can be placed in the packets' path anywhere in the network. However, certain modules, such as interfaces, have optional :ned:`MeasurementLayer` submodules. The :ned:`MeasurementLayer` module contains both measurment modules, and can be places easily in the network.

  .. note:: FlowMeasurmentStarter and FlowMeasurmentRecorder modules can be placed in any submodule by creating an extended version of that module. See the TODO section for an example.

Adding Measurement Modules
~~~~~~~~~~~~~~~~~~~~~~~~~~

The FlowMeasurementStarter and FlowMeasurementRecorder modules can be inserted anywhere in the network (inside network nodes or protocol modules, etc) in NED by editing the NED source of modules or extending them as a new type.
However, some modules such as the LayeredEthernetInterface, have optional slots for a :ned:`MeasurementLayer` module. This module contains a FlowMeasurementStarter and a FlowMeasurementRecorder, and can be inserted from the .INI file (e.g. ``*.host.eth[0].measurementLayer.typename = "MeasurementLayer"``).

.. figure:: media/Default_MeasurementLayer.png
   :align: center

   Figure X. :ned:`MeasurementLayer`

Classifying Packets Using Composition of Measurement Modules
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A measurement module can enter packets that match its filter to a certain flow. However, for classifying packets to multiple flows based on multiple criteria,
one can use several measurement modules connected serially. The MultiMeasurementLayer module makes this more convenient. It can be used in place of a MeasurementLayer module. It contains a variable number of MeasurementStarter and FlowMeasurementMaker modules; the number of modules is specified with its :par:`numMeasurementModules` parameter. For example, ``numMeasurementModules = 2``:

.. **TODO** For example, a MultiMeasurementLayer module with 4 sets of measurement modules

.. figure:: media/MultiMeasurementLayer.png
   :align: center
   
   Figure X. :ned:`MultiMeasurementLayer` with two sets of measurement modules

.. **TODO** it can be used in place of a MeasurementLayer module

The example simulations demonstrate inserting measurement modules into both specific locations (into the :ned:`Udp` module) and optional :ned:`MeasurementLayer` slots.

Limitations
~~~~~~~~~~~

Currently, packet flows aren't supported when using fragmentation or frame aggregation.

.. **TODO** limitations

.. **TODO** packetFlowVisualizer

Visualizing Packet Flows
~~~~~~~~~~~~~~~~~~~~~~~~

:ned:`PacketFlowVisualizer` (also included in :ned:`IntegratedVisualizer`) can display packet flows in the network as dashed arrows annotated by the flow name. The arrows are color-coded so that flows can be differentiated. The visualization can be enabled with the :par:`displayPacketFlows` parameter, e.g. ``*.visualizer.packetFlowVisualizer.displayPacketFlows = true``.

The Example Simulations
-----------------------

The showcase contains the following examples simulations:

- TODO: Demonstrates creating packet flows by putting measurement modules into optional slots
- TODO: Demonstrates adding measurement modules to any module

.. FlowMeasurementStarter and FlowMeasurementRecorder

Both simulations use the following network:

.. figure:: media/Network.png
   :align: center

It contains :ned:`StandardHost`'s connected by :ned:`EthernetSwitch`'s in a dumbbell topology.

In both simulations, each client sends packets to the two servers. The simulations differ only in what packet flows and measurements are defined.

Config: Measurement Modules in Optional Slots
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this simulation, we'll put a measurement thing in the UDP app and the switches.

In this simulation, we'll specify and measure five packet flows:

- 4 packet flows going from each client to the two servers (client1->server1, client1->server2, client2->server1, client2->server2)
- A packet flow going from switch1 to switch2

.. note:: The four packet flows originating in the hosts "meet" at switch1, so that the switch1 packet flow overlaps with all four of them

so

- we put the measurement thing in the UDP app
- also in the switch
- we measure what
- how to configure that
- illustrate the flows

.. figure:: media/flows2.png
   :align: center