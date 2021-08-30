:orphan:

Measuring Time Along Packet Flows
=================================

Goals
-----

This showcase demonstrates measuring time associated with a packet as it travels/when it's present in a network, such as total elapsed time, time spent in queues, or transmission time. 

This showcase demonstrates timing measurments along packet flows, i.e. paths various packets take in the network.

The Model
---------

The following sections contain an overview of packet flows. For more information, refer to the **TODO** section of the INET manual.

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

For timing measurements that require access to data of multiple modules, `packet flows` can be defined. A packet flow is a logical classification of packets, identified by its name, in the whole network and over the whole duration of the simulation. Flows are defined per-bit, i.e. each bit of the packet is kept track of in regard to which flow it belongs to (as continuous regions of the packet). Flows are defined by active modules which create entry and exit points of the flow in the network. The following are some more properties of packet flows:

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
However, some modules such as the LayeredEthernetInterface, have optional :ned:`MeasurementLayer` submodules. This module contains a FlowMeasurementStarter and a FlowMeasurementRecorder, and can be inserted from the .INI file (e.g. ``*.host.eth[0].measurementLayer.typename = "MeasurementLayer"``).

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

The example simulations demonstrate both inserting measurement modules into specific locations (below the :ned:`Udp` module) and using optional :ned:`MeasurementLayer` submodules already present in some modules.

Limitations
~~~~~~~~~~~

Currently, packet flows aren't supported when using fragmentation or frame aggregation.

.. **TODO** limitations

.. **TODO** packetFlowVisualizer

Visualizing Packet Flows
~~~~~~~~~~~~~~~~~~~~~~~~

:ned:`PacketFlowVisualizer` (also included in :ned:`IntegratedVisualizer`) can display packet flows in the network as dashed arrows annotated by the flow name. The arrows are color-coded so that flows can be differentiated. The visualization can be enabled with the :par:`displayPacketFlows` parameter, e.g. ``*.visualizer.packetFlowVisualizer.displayPacketFlows = true``.

.. **TODO** link to manual

.. **TODO** its per-bit

.. **TODO** note about the elapsedTime not the sum of the others

The Example Simulations
-----------------------

The showcase contains the following examples simulations:

.. **TODO** Basic-Advanced / OptionalSlots...

.. **TODO** ide fejezetcim

.. /putting measurement modules as optional submodules

- **Adding Measurement Modules as Optional Submodules** (``Basic`` config): Demonstrates creating packet flows by using optional measurement submodules in modules which contain them
- **Putting Measurement Modules into Any Module** (``AnyLocation`` config): Demonstrates the following: 
  
  - Adding measurement modules to an arbitrary module
  - Putting packets from multiple sources into the same flow
  - Classifying packets into flows based on port numbers

.. FlowMeasurementStarter and FlowMeasurementRecorder

Both simulations use the following network:

.. figure:: media/Network.png
   :align: center

   Figure X. The network

It contains hosts connected by switches (:ned:`EthernetSwitch`) in a dumbbell topology. Note that the type of the hosts is parameterized (so that it can be overridden from the ini file):

.. **TODO**

.. literalinclude:: ../FlowMeasurementShowcase.ned
   :start-at: client1
   :end-at: {
   :language: ned

.. In both simulations, each client sends packets to the two servers. The simulations differ only in what packet flows and measurements are defined.

Adding Measurement Modules as Optional Submodules
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Configuration
+++++++++++++++++

.. In this simulation, we'll put a measurement thing in the UDP app and the switches.

.. **TODO** we use StandardHost here

In this simulation, the type of the hosts is :ned:`StandardHost`. Two UDP apps in both clients send packets to the servers (``app[0]`` to ``server1``, ``app[1]`` to ``server2``).
Each server has a UDP app that accepts the packets. We specify and measure five packet flows:

- Four packet flows going from each client to the two servers (``client1`` -> ``server1``, ``client1`` -> ``server2``, ``client2`` -> ``server1``, ``client2`` -> ``server2``)
- A packet flow going from ``switch1`` to ``switch2``

.. note:: The four packet flows originating in the hosts "meet" at ``switch1``, so that the packet flow between the switches (``switch1`` flow) overlaps with all four of them

.. so

  - we put the measurement thing in the UDP app
  - also in the switch
  - we measure what
  - how to configure that
  - illustrate the flows

The specified flows are visualized on the following image:

.. figure:: media/flows2.png
   :align: center

   Figure X. The specified packet flows

The UDP apps in the clients and servers are the :ned:`UdpApp` type, i.e. the modular UDP app. This module has an optional ``outbound`` module that we can specify in the .INI file to be a :ned:`FlowMeasurementStarter` in the clients:

.. literalinclude:: ../omnetpp.ini
   :start-at: *.client1.app[*].outbound.typename = "FlowMeasurementStarter"
   :end-at: *.client1.app[*].outbound.typename = "FlowMeasurementStarter"
   :language: ini

Here is the :ned:`FlowMeasurementStarter` in the UDP app:

.. figure:: media/Default_UdpApp3.png
   :align: center

   Figure X. :ned:`FlowMeasurementStarter` in UDP app

Similarly, we specify the inbound module in the server UDP apps to be a :ned:`FlowMeasurementRecorder`:

.. literalinclude:: ../omnetpp.ini
   :start-at: *.server1.app[*].inbound.typename = "FlowMeasurementRecorder"
   :end-at: *.server1.app[*].inbound.typename = "FlowMeasurementRecorder"
   :language: ini

For the packet flow between the two switches, we can use the optional ``measurementLayer`` submodule of :ned:`EthernetInterface`:

.. literalinclude:: ../omnetpp.ini
   :start-at: *.switch*.eth[2].measurementLayer.typename = "MeasurementLayer"
   :end-at: *.switch*.eth[2].measurementLayer.typename = "MeasurementLayer"
   :language: ini

.. figure:: media/EthernetInterface.png
   :align: center

   Figure X. ``measurementLayer`` in :ned:`EthernetInterface`

Here is the complete flow definition configuration (including the definitions already mentioned):

.. literalinclude:: ../omnetpp.ini
   :start-at: *.client1.app[*].outbound.typename = "FlowMeasurementStarter"
   :end-at: *.switch2.eth[2].measurementLayer.measurementMaker.measure = "elapsedTime or queueingTime"
   :language: ini

.. **TODO** explain

  - the flownamesFlows can overlap in time and in network topology

  Flows can have multiple entry and exit points
  
  Certain packets are classified to be entering the flow by FlowMeasurementStarter modules, for example by matching a filter. The modules add a FlowTag that describes the kind of measurements requested by the user. Based on the FlowTag, the measurements are added to the packet as metadata by certain modules as it travels in the network.
  
  For example, we specify queueing time to be measured. The queues along the packet’s route add the measured queueing time to the packet’s metadata. Packets exit the flow at FlowMeasurementRecorder modules. The measurements are recorded there as the FlowMeasurementRecorder module’s statistics, containing the total queueing time incurred by the packet.
  
  The FlowMeasurementStarter and FlowMeasurementRecorder modules have the same set 
  - what we measure
  - need to be the same in both modules (is that above?)
  - only need one kind of module, but it doesnt matter
  - only need in eth2 in switches

.. **TODO** possible to record multiple flows in a recorder

We set up the four flows between the clients and the servers, and also the flow between the two switches. We name flows based on source module (e.g. ``client1``). We set the measurement modules to measure the elapsed time and the queueing time. Some notes:

- The packets from the two apps in a client belong to the same flow, it just has multiple exit points (``server1`` and ``server2``). 
- The :ned:`FlowMeasurementRecorder` in the servers record flows from both clients. 
- The ``switch1`` flow is between the two switch interfaces facing each other (``eth2`` in both switches), so that it's unnecessary for the other switch interfaces to have measurement modules.
- | It is possible to record multiple flows in a FlowMeasurementRecorder, e.g.: 
  | ``*.server1.app[*].inbound.flowName = "client1 or client2"``.

Results
+++++++

**TODO** Multiple exit points for the same flow -> recorded independently

**TODO** some charts? -> histograms! ha van valami ertelmes histogram

.. figure:: media/anylocation_server2_elapsedtime_histogram.png
   :align: center

Putting Measurement Modules into Any Module
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Configuration
+++++++++++++++++

In this configuration, we want to create packet flows below the :ned:`Udp` module in hosts. Compared to the previous configuration where each app had its own flow,
here packets from the two application are mixed in the flows.

.. **TODO** ossze fog mixelodni a 2 application (eddig az appban volt)

As mentioned above, in this configuration, we want to add a measurement module to the network without using optional measurement submodules already present in some modules.
To demonstrate that, we create packet flows below the :ned:`Udp` module in hosts. 

The easiest way to insert a measurement module into any module (which doesn't already have an optional submodule) is to extend with a measurement module as a new type. For example, we extend :ned:`StandardHost` into ``MyStandardHost`` in FlowMeasurementShowcase.ned. We could create two versions, one with a :ned:`FlowMeasurementStarter` and one with a :ned:`FlowMeasurementRecorder`, but it is more convenient and more generic to add a :ned:`MeasurementLayer`, which contains both:

.. literalinclude:: ../FlowMeasurementShowcase.ned
   :start-at: MyStandardHost
   :end-before: FlowMeasurementShowcase
   :language: ned

To make ``MyStandardHost`` even more generic, we make the measurement layer module optional (empty string by default). Note that we just added an optional measurement layer submodule. We can add a measurement layer module from the .INI file.

We insert the measurement layer module between the :ned:`Udp` module and the :ned:`MessageDispatcher` below it. The gates need to be reconnected to the new module (as per the connection section in the NED code above).
Here is the result:

.. figure:: media/MyStandardHost.png
   :align: center

   Figure X. The Contents of ``MyStandardHost``

Now, we can use the ``MyStandardHost`` type in the .INI configuration:

.. literalinclude:: ../omnetpp.ini
   :start-at: MyStandardHost
   :end-at: server
   :language: ini

We want to demonstrate classification of packets into multiple packet flows based on source port. To do that, we need multiple :ned:`FlowMeasurementStarter` modules, each entering packets to different a flow. Conveniently, we can use the :ned:`MultiMeasurementLayer` module in the hosts:

.. literalinclude:: ../omnetpp.ini
   :start-at: MultiMeasurementLayer
   :end-at: server
   :language: ini

Our goal is to enter UDP packets with source port 500 to flow ``VID``, and those with source port 1000 to flow ``BG``. Thus, we need two measurement starter modules:

.. **TODO** do we need two recorders? yes. because there is classification in the server side as well. -> actually, do we need packet data filters in the servers?
   if not, one recorder might be enough -> try, but no need for classification on the server side

.. literalinclude:: ../omnetpp.ini
   :start-at: numMeasurementModules
   :end-at: numMeasurementModules
   :language: ini

.. note:: One measurement recorder module is enough in the servers, because it doesn't need to classify packets (already classified), it just needs to record both flows.

.. note:: One measurement recorder module is enough in the servers, as it can record both flows.

Here is the flow definition in omnetpp.ini:

.. literalinclude:: ../omnetpp.ini
   :start-at: *.client*.measurementLayer.measurementStarter[0].flowName = "VID"
   :language: ini

.. **TODO** explain

  - flow names
  - packet data filter
  - measurements
  - omitted type
  - how its putting it in the same flow

We set up two flows (``BG`` and ``VID``) based on the source port of packets. The flows have multiple entry and exit points (in both clients and servers, respectively). Each measurement module in in MultiMeasurementLayer classifies packets to one of the flows, using the packet data filter. Just as in the previous configuration, we measure elapsed time and queueing time. Note that:

- The recorders record flows independently
- Packets from multiple sources that belong to the same flow are recorded in the same statistic
- We don't need the measurement recorders in the clients, and the measurement starters in the servers, so we disable them.

..  -> nem kell mar az omitted...ha uresen hagyod akkor omitted lesz TRY

.. **TODO** whats the difference between flownames = client1 or client2 and each recorder recording a different flow? -> seems like they are recorded to two statistics

**TODO** to put packets from multiple sources to the same flow, just specify the same flowname; also, the different flow exit points have different statistics for the same flow -> but not for the same flow with different entry points

   basically: 

   - a flow can have multiple entry and exit points
   - at exit points, each flow is recorded to its own statistic

**TODO** ? lehetne kulon a recording es az exit

Results
+++++++

The measured elapsed time and queueing time values are available as the statistics of the measurement recorder modules. For example, here is the two elapsed time vectors (for the ``BG`` and ``VID`` flows) recorded by the measurement modules in ``server1``, displayed in the IDE's analysis tool:

.. figure:: media/BrowseData.png
   :align: center

   Figure X. Elapsed time vectors in ``server1``'s measurement layer

Here is the same data selection plotted on a scatter chart:

.. figure:: media/anylocation_server1_elapsedtime.png
   :align: center

   Figure X. Plot of elapsed time in ``server1``

.. **TODO** how to check flowtags on packets

Checking which Flows Packets Belong to
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. **TODO** here is an example from the first config

To check which flows a packet belongs to, one can take a look at the ``FlowTag``'s attached to the packet in Qtenv's object inspector. One way to do that is to select the packet, and switch the object inspector view to `Flat` mode, and open the encapsulated packet element. Here is an example from the ``Default`` simulation:

.. figure:: media/flowtag1.png
   :align: center

   Figure X. A packet in the object inspector, flat mode

.. Under the encapsulated packet, open the `Region tags` element, and a FlowTag:

Under the encapsulated packet, open `Region tags`, open a FlowTag; the names array contains the flow names the packet belongs to (``client2`` and ``switch1`` in this case):

.. figure:: media/flowtag3.png
   :align: center

   Figure X. FlowTag attached to the packet

Flows are defined per-bit of the packet, thus the flow tags are attached as region tags, i.e. they are attached to a certain part of the packet. Here are all the region tags for the packet:

.. figure:: media/flowtag2.png
   :align: center

   Figure X. Region tags attached to the packet

In this case, there are three region tags attached to bytes 42-1042, and three more to bytes 0-42. (the ``ElapsedTimeTag`` and ``QueueingTimeTag`` contains the elapsed and queieing times, the ``FlowTag`` contains the flow name). The first three tags are added by the measurement module under the ``udp`` module in the hosts. The packet is just the data payload (1000B) at this point, without UDP, IP and Ethernet encapsulation (42B headers total). The last three region tags are added in switch1, when the packet already has those headers. Consequently, the second flow tag only contains the switch1 flow, because that part of the packet didn't exist when the client added its flowtag/at the client's measurement module.

.. **TODO** a szoveg legyen jo, a kep rossz, es csak a kepet kell kicserelni

.. **TODO** why 1000-1042? shouldnt it be on the front? github issue #716; note: most nem mukodik de a magyarazat helytallo

.. note:: From the image above, it appears that the headers are added to the end of the packet, instead of the beginning. This is a bug yet to be fixed, but the above explanation is correct.

**TODO** replace image when github issue #716 is fixed

Components of the `ElapsedTime`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``ElapsedTime`` measurement measures the total time spent in the flow. The other measurements (QueueingTime, ProcessingTime, etc.) measure the specific events within the elapsed time. However, the sum of all these specific measurements might be less than the elapsed time for a particular packet, because the measurements are per-bit. For example, if there is packet streaming in the network, some bits are transmitted later than others. In the meantime, they "wait" in the transmitting module (e.g. streaming Ethernet Interface) for transmission of previous bits, and also in the receiving interface for reception of subsequent bits. This wait time is only contained in the elapsed time measurement.

The `PacketEvent` Measurement
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**TODO** need c++ code to use it in a meaningful way