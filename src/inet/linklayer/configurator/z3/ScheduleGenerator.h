#ifndef __INET_Z3_SCHEDULEGENERATOR_H
#define __INET_Z3_SCHEDULEGENERATOR_H

#include <z3++.h>

#include "inet/linklayer/configurator/z3/Defs.h"
#include "inet/linklayer/configurator/z3/Switch.h"

namespace inet {

using namespace z3;

/**
 * [Class]: ScheduleGenerator
 * [Usage]: Used to generate a schedule based on the properties of
 * a given network through the method generateSchedule. Will create
 * a log file and store the timing properties on the cycles and flows.
 */
class INET_API ScheduleGenerator {
  public:
//       @SuppressWarnings("serial")
//       class TestFailedException extends Exception
//       {
//           TestFailedException()
//           {
//               super("Check FAILED");
//           }
//       };

       /**
        * [Method]: stringToFloat
        * [Usage]: After evaluating the model, z3 allows the
        * user to retrieve the values of variables. This
        * values are stored as strings, which are converted
        * by this function in order to be stored in the
        * classes variables. Often these z3 variables are
        * also on fraction form, which is also handled by
        * this function.
        *
        * @param str   std::string containing value to convert to double
        * @return      double value of the given string str
        */
       double stringToFloat(std::string str) {
//           BigDecimal val1;
//           BigDecimal val2;
           double result = 0;

           int index = str.find('/');
           if(index != std::string::npos) {
               str = str.substr(3, str.length() - 1); // (/ a b)
               index = str.find(' ');
               double val1 = atof(str.substr(0, index).c_str());
               double val2 = atof(str.substr(index + 1).c_str());
               result = val1 / val2;
//               val1 = new BigDecimal(str.split("/")[0]);
//               val2 = new BigDecimal(str.split("/")[1]);
//               result = val1.divide(val2, MathContext.DECIMAL128).doubleValue();
//
           } else {
//               try{
                     result = atof(str.c_str());
//                }catch(NumberFormatException e){
//                    result = -1;
//                }
           }
//
           return result;
       }


       /**
        * [Method]: createContext
        * [Usage]: Returns a z3 context used as an environment
        * for creation of z3 variables and operations
        *
        * @return  A z3 context
        */
       context *createContext() {
           std::cout << "findSchedulerModel\n";

//           try
//           {
//                com.microsoft.z3.Global.ToggleWarningMessages(true);
                std::cout << "Z3 Major Version: " << std::endl;
//                std::cout << version.getMajor() << std::endl;
                std::cout << "Z3 Full Version: " << std::endl;
//                std::cout << version.getString() << std::endl;
                std::cout << "Z3 Full Version std::string: " << std::endl;
//                std::cout << version.getFullVersion() << std::endl;
                std::cout << "" << std::endl;


                { // These examples need model generation turned on.
                    config cfg;
                    cfg.set("model", "true");
                    context *ctx = new context(cfg);

                    return ctx;
                }
//            } catch (Z3Exception ex)
//            {
//                std::cout << std::string("Z3 Managed Exception: ") + ex.getMessage());
//                std::cout << "Stack trace: ");
//                ex.printStackTrace(System.out);
//            } catch (Exception ex)
//            {
//                std::cout << std::string("Unknown Exception: ") + ex.getMessage());
//                std::cout << "Stack trace: ");
//                ex.printStackTrace(System.out);
//            }
//
            return nullptr;
       }


       /**
        * [Method]: closeContext
        * [Usage]: Clears and close the context used to
        * generate the schedule.
        *
        * @param ctx   context to be cleared
        */
       void closeContext(context& ctx) {
//           try
//           {
//
//                {
//                    ctx.close();
//                }
//
//                //Log.close();
//                if (Log.isOpen())
//                    std::cout << "Log is still open!");
//            } catch (Z3Exception ex)
//            {
//                std::cout << std::string("Z3 Managed Exception: ") + ex.getMessage());
//                std::cout << "Stack trace: ");
//                ex.printStackTrace(System.out);
//            } catch (Exception ex)
//            {
//                std::cout << std::string("Unknown Exception: ") + ex.getMessage());
//                std::cout << "Stack trace: ");
//                ex.printStackTrace(System.out);
//            }
       }

       /**
        * [Method]: writePathTree
        * [Usage]: This is a recursive function used to
        * navigate through the pathTree, storing information
        * about the switches and flowFramengts in the nodes
        * and printing data in the log.
        *
        * @param pathNode  Current node of pathTree (should start with root)
        * @param model     Output model generated by z3
        * @param ctx       z3 context used to generate the model
        * @param out       PrintWriter stream to output log file
        */
       void writePathTree(PathNode *pathNode, model& model, context& ctx) {
           Switch *swt;
           std::shared_ptr<expr> indexZ3 = nullptr;

           if(dynamic_cast<Device *>(pathNode->getNode()) && (pathNode->getParent() != nullptr)) {
               std::cout << "    [END OF BRANCH]" << std::endl;

           }


           /*
            * Once given a node, an iteration through its children will begin. For
            * each switch children, there will be a flow fragment, and to each device
            * children, there will be an end of branch.
            *
            * The logic for storing and printing the data on the publish subscribe
            * flows is similar but easier than the unicast flows. The pathNode object
            * stores references to both flow fragment and switch, so no search is needed.
            */
           for(PathNode *child : pathNode->getChildren()) {
               if(dynamic_cast<Switch *>(child->getNode())) {

                   for(FlowFragment *ffrag : child->getFlowFragments()) {
                       std::cout << std::string("    Fragment name: ") << ffrag->getName() << std::endl;
                       std::cout << std::string("        Fragment node: ") << ffrag->getNodeName() << std::endl;
                       std::cout << std::string("        Fragment next hop: ") << ffrag->getNextHop() << std::endl;
                       std::cout << std::string("        Fragment priority: ") << model.eval(*ffrag->getFragmentPriorityZ3(), false) << std::endl;
                       for(int index = 0; index < ((Switch *) child->getNode())->getPortOf(ffrag->getNextHop())->getCycle()->getNumOfSlots(); index++) {
                           indexZ3 = std::make_shared<expr>(ctx.int_val(index));
                           std::cout << std::string("        Fragment slot start ") << index << std::string(": ")
                                   << this->stringToFloat(
                                           model.eval(*((Switch *) child->getNode())
                                                  ->getPortOf(ffrag->getNextHop())
                                                  ->getCycle()
                                                  ->slotStartZ3(ctx, *ffrag->getFragmentPriorityZ3(), *indexZ3)
                                                  , false).to_string()) << std::endl;
                           std::cout << std::string("        Fragment slot duration ") << index << std::string(" : ")
                                    << this->stringToFloat(
                                        model.eval(*((Switch *) child->getNode())
                                                   ->getPortOf(ffrag->getNextHop())
                                                   ->getCycle()
                                                   ->slotDurationZ3(ctx, *ffrag->getFragmentPriorityZ3(), *indexZ3)
                                                   , false).to_string()) << std::endl;

                       }

                       std::cout << "        Fragment times-" << std::endl;
                       ffrag->getParent()->addToTotalNumOfPackets(ffrag->getNumOfPacketsSent());

                       for(int i = 0; i < ffrag->getParent()->getNumOfPacketsSent(); i++) {
                               std::cout << std::string("On " + ffrag->getName() + std::string(" - ")) <<
                                ((Switch *)child->getNode())->departureTime(ctx, i, ffrag)->to_string() << std::string(" - ") <<
                                  ((Switch *)child->getNode())->scheduledTime(ctx, i, ffrag)->to_string() << std::endl;
                           // std::cout << ((TSNSwitch *)child->getNode())->departureTime(ctx, i, ffrag).to_string() << std::endl;
                           // std::cout << ((TSNSwitch *)child->getNode())->arrivalTime(ctx, i, ffrag).to_string() << std::endl;
                           // std::cout << ((TSNSwitch *)child->getNode())->scheduledTime(ctx, i, ffrag).to_string() << std::endl;

                           if(i < ffrag->getParent()->getNumOfPacketsSent()) {
                               std::cout << std::string("          (") << std::to_string(i) << std::string(") Fragment departure time: ") << this->stringToFloat(model.eval(*((Switch *) child->getNode())->departureTime(ctx, i, ffrag), false).to_string()) << std::endl;
                               std::cout << std::string("          (") << std::to_string(i) << std::string(") Fragment arrival time: ") << this->stringToFloat(model.eval(*((Switch *) child->getNode())->arrivalTime(ctx, i, ffrag), false).to_string()) << std::endl;
                               std::cout << std::string("          (") << std::to_string(i) << std::string(") Fragment scheduled time: ") << this->stringToFloat(model.eval(*((Switch *) child->getNode())->scheduledTime(ctx, i, ffrag), false).to_string()) << std::endl;
                               std::cout << "          ----------------------------" << std::endl;
                           }

                           ffrag->setFragmentPriority(
                               atoi(
                                   model.eval(*ffrag->getFragmentPriorityZ3(), false).to_string().c_str()
                               )
                           );

                           ffrag->addDepartureTime(
                               this->stringToFloat(
                                   model.eval(*((Switch *) child->getNode())->departureTime(ctx, i, ffrag) , false).to_string()
                               )
                           );
                           ffrag->addArrivalTime(
                               this->stringToFloat(
                                   model.eval(*((Switch *) child->getNode())->arrivalTime(ctx, i, ffrag) , false).to_string()
                               )
                           );
                           ffrag->addScheduledTime(
                               this->stringToFloat(
                                   model.eval(*((Switch *) child->getNode())->scheduledTime(ctx, i, ffrag) , false).to_string()
                               )
                           );

                       }

                       swt = (Switch *) child->getNode();

                       for (Port *port : ((Switch *) swt)->getPorts()) {

                           auto flowFragments = port->getFlowFragments();
                           if(std::find(flowFragments.begin(), flowFragments.end(), ffrag) == flowFragments.end()) {
                               continue;
                           }

                           std::vector<double> listOfStart;
                           std::vector<double> listOfDuration;


                           for(int index = 0; index < ((Switch *) child->getNode())->getPortOf(ffrag->getNextHop())->getCycle()->getNumOfSlots(); index++) {
                               indexZ3 = std::make_shared<expr>(ctx.int_val(index));

                               listOfStart.push_back(
                                   this->stringToFloat(model.eval(
                                       *((Switch *) child->getNode())
                                       ->getPortOf(ffrag->getNextHop())
                                       ->getCycle()->slotStartZ3(ctx, *ffrag->getFragmentPriorityZ3(), *indexZ3) , false).to_string())
                               );
                               listOfDuration.push_back(
                                   this->stringToFloat(model.eval(
                                       *((Switch *) child->getNode())
                                       ->getPortOf(ffrag->getNextHop())
                                       ->getCycle()->slotDurationZ3(ctx, *ffrag->getFragmentPriorityZ3(), *indexZ3) , false).to_string())
                               );
                           }

                           port->getCycle()->addSlotUsed(
                               (int) this->stringToFloat(model.eval(*ffrag->getFragmentPriorityZ3(), false).to_string()),
                               listOfStart,
                               listOfDuration
                           );
                       }

                   }

                   this->writePathTree(child, model, ctx);
               }
           }

       }


       void configureNetwork(Network *net, context& ctx, solver& solver) {
           for(Flow *flw : net->getFlows()) {
                  flw->modifyIfUsingCustomVal();
               flw->convertUnicastFlow();
               flw->setUpPeriods(flw->getPathTree()->getRoot());
           }

           for(Switch *swt : net->getSwitches()) {
               swt->setUpCycleSize(solver, ctx);
           }


           // On all network flows: Data given by the user will be converted to z3 values
           for(Flow *flw : net->getFlows()) {
               flw->toZ3(ctx);
               flw->setNumberOfPacketsSent(flw->getPathTree()->getRoot());
           }

           // On all network switches: Data given by the user will be converted to z3 values
           for(Switch *swt : net->getSwitches()) {
               swt->toZ3(ctx, solver);
           }

           // Sets up the hard constraint for each individual flow in the network
           net->setJitterUpperBoundRangeZ3(ctx, 25);
           net->secureHC(solver, ctx);
       }


       /**
        * [Method]: generateSchedule
        * [Usage]: After creating a network, setting up the
        * flows and switches, the user now can call this
        * function in order calculate the schedule values
        * using z3
        *
        * @param net   Network used as base to generate the schedule
        */
       void generateSchedule(Network *net)
       {
           context *ctx = this->createContext(); //Creating the z3 context
           solver solver(*ctx);     //Creating the solver to generate unknown values based on the given context


            this->configureNetwork(net, *ctx, solver);
           // net.loadNetwork(ctx, solver);


           // A switch is picked in order to evaluate the unknown values
           Switch *switch1 = nullptr;
           switch1 = (Switch *) net->getSwitches().at(0);
           // The duration of the cycle is given as a question to z3, so all the
           // constraints will have to be evaluated in order to z3 to know this cycle
           // duration
           std::shared_ptr<expr> switch1CycDuration = switch1->getCycle(0)->getCycleDurationZ3();


           /* find model for the constraints above */
//           model model = nullptr;
//           LocalTime time = LocalTime.now();

           std::cout << "Rules set. Checking solver." << std::endl;
           std::cout << solver << std::endl;
//           std::cout << std::string("Current time of the day: ") << time << std::endl;

           if (sat == solver.check())
           {
               model model = solver.get_model();
               std::cout << model << std::endl;
               expr v = model.eval(*switch1CycDuration, false);
//               if (v != nullptr)
//               {
                   std::cout << "Model generated." << std::endl;

//                   try {
//                       PrintWriter out = new PrintWriter("log.txt");

                       std::cout << "SCHEDULER LOG:\n\n" << std::endl;

                       std::cout << "SWITCH LIST:" << std::endl;

                       // For every switch in the network, store its information in the log
                       for(Switch *auxSwt : net->getSwitches()) {
                           std::cout << std::string("  Switch name: ") << auxSwt->getName();
                           std::cout << std::string("    Max packet size: ") << auxSwt->getMaxPacketSize();
                           std::cout << std::string("    Port speed: ") << auxSwt->getPortSpeed();
                           std::cout << std::string("    Time to Travel: ") << auxSwt->getTimeToTravel();
                           std::cout << std::string("    Transmission time: ") << auxSwt->getTransmissionTime();
//                           std::cout << "    Cycle information -" << std::endl;
//                           std::cout << std::string("        First cycle start: ") + model.eval(((TSNSwitch *)auxSwt).getCycleStart(), false));
//                           std::cout << std::string("        Cycle duration: ") + model.eval(((TSNSwitch *)auxSwt).getCycleDuration(), false));
                           std::cout << "" << std::endl;
                           /*
                           for (Port port : ((TSNSwitch *)auxSwt).getPorts()) {
                               std::cout << std::string("        Port name (Virtual Index): ") + port->getName());
                               std::cout << std::string("        First cycle start: ") + model.eval(port->getCycle()->getFirstCycleStartZ3(), false));
                               std::cout << std::string("        Cycle duration: ") + model.eval(port->getCycle()->getCycleDurationZ3(), false));
                               std::cout << "" << std::endl;
                           }
                           */


                           // [EXTRACTING OUTPUT]: Obtaining the z3 output of the switch properties,
                           // converting it from string to double and storing in the objects

                           for (Port *port : ((Switch *)auxSwt)->getPorts()) {
                               port
                                   ->getCycle()
                                   ->setCycleStart(
                                       this->stringToFloat(model.eval(*port->getCycle()->getFirstCycleStartZ3(), false).to_string())
                                   );
                           }

                           // cycleDuration
                           for (Port *port : ((Switch *)auxSwt)->getPorts()) {
                               port
                                   ->getCycle()
                                   ->setCycleDuration(
                                       this->stringToFloat(model.eval(*port->getCycle()->getCycleDurationZ3(), false).to_string())
                                   );
                           }

                       }

                       std::cout << "" << std::endl;

                       std::cout << "FLOW LIST:" << std::endl;
                       //For every flow in the network, store its information in the log
                       for(Flow *f : net->getFlows()) {
                           std::cout << std::string("  Flow name: ") + f->getName() << std::endl;
                           // std::cout << std::string("    Flow priority: ") + model.eval(f->getFlowPriority(), false) << std::endl;
                           std::cout << std::string("    Start dev. first t1: ") << model.eval(*f->getStartDevice()->getFirstT1TimeZ3(), false).to_string() << std::endl;
                           std::cout << std::string("    Start dev. HC: ") << model.eval(*f->getStartDevice()->getHardConstraintTimeZ3(), false).to_string() << std::endl;
                           std::cout << std::string("    Start dev. packet periodicity: ") << model.eval(*f->getStartDevice()->getPacketPeriodicityZ3(), false).to_string() << std::endl;

                           f->setFlowFirstSendingTime(stringToFloat(model.eval(*f->getStartDevice()->getFirstT1TimeZ3(), false).to_string()));

                           // IF FLOW IS UNICAST
                           /*
                           Observation: The flow is broken in smaller flow fragments.
                           In order to know the departure, arrival, scheduled times
                           and other properties of the flow the switch that the flow fragment
                           belongs to must be retrieved. The flow is then used on the switch
                           to find the port to its destination. The port and the flow fragment
                           can now be used to retrieve information about the flow.

                           The way in which unicast and publish subscribe flows are
                           structured here are different. So this process is done differently
                           in each case.
                           */
                           if(f->getType() == Flow::UNICAST) {
                               // TODO: Throw error. UNICAST data structure are not allowed at this point
                               // Everything should had been converted into the multicast model.
                           } else if(f->getType() == Flow::PUBLISH_SUBSCRIBE) { //IF FLOW IS PUB-SUB

                               /*
                                * In case of a publish subscribe flow, it is easier to
                                * traverse through the path three than iterate over the
                                * nodes as it could be done with the unicast flow.
                                */

                               PathTree *pathTree;
                               PathNode *pathNode;

                               pathTree = f->getPathTree();
                               pathNode = pathTree->getRoot();

                               std::cout << "    Flow type: Multicast" << std::endl;
                               std::vector<PathNode *> *auxNodes;
                               std::vector<FlowFragment *> *auxFlowFragments;
                               int auxCount = 0;

                               std::cout << "    List of leaves: ";
                               for(PathNode *node : *f->getPathTree()->getLeaves()) {
                                   std::cout << ((Device *) node->getNode())->getName() + std::string(", ");
                               }
                               std::cout << "" << std::endl;
                               for(PathNode *node : *f->getPathTree()->getLeaves()) {
                                   auxNodes = f->getNodesFromRootToNode((Device *) node->getNode());
                                   auxFlowFragments = f->getFlowFromRootToNode((Device *) node->getNode());

                                   std::cout << std::string("    Path to ") + ((Device *) node->getNode())->getName() + std::string(": ");
                                   auxCount = 0;
                                   for(PathNode *auxNode : *auxNodes) {
                                       if(dynamic_cast<Device *>(auxNode->getNode())) {
                                           std::cout << ((Device *) auxNode->getNode())->getName() + std::string(", ");
                                       } else if (dynamic_cast<Switch *>(auxNode->getNode())) {
                                           std::cout <<
                                               ((Switch *) auxNode->getNode())->getName() +
                                               std::string("(") +
                                               auxFlowFragments->at(auxCount)->getName() +
                                               "), ";
                                           auxCount++;
                                       }

                                   }
                                   std::cout << "" << std::endl;
                               }
                               std::cout << "" << std::endl;

                               //Start the data storing and log printing process from the root
                               this->writePathTree(pathNode, model, *ctx);
                           }

                           std::cout << "" << std::endl;

                       }
//
//                   } catch (FileNotFoundException e) {
//                       e.printStackTrace();
//                   }

//               } else
//               {
//                   std::cout << "Failed to evaluate" << std::endl;
//               }
           } else
           {
               std::cout << solver.unsat_core() << std::endl;
               throw cRuntimeError("The specified constraints might not be satisfiable.");
           }

//           this->closeContext(ctx);
//           new XMLExporter(net);
           // this->serializeNetwork(net, "network.ser");
       }


       /**
        * [Method]: serializateNetwork
        * [Usage]: Serialize the primitive objects of the network object.
        * The serialized file is stored in the path string folder. Can be used
        * to store the data of a network and the values of the generated
        * schedule.
        *
        * @param net        Network object to be serialized
        * @param path        Path of for the serialized object file
        */
//       public void serializeNetwork(Network net, std::string path) {
//
//            try {
//                FileOutputStream fileOut = new FileOutputStream(path);
//                ObjectOutputStream out = new ObjectOutputStream(fileOut);
//                out.writeObject(net);
//                out.close();
//                fileOut.close();
//                System.out.printf("Serialized data is saved in network.ser");
//             } catch (Exception i) {
//                i.printStackTrace();
//             }
//        }

       /**
        * [Method]: deserializeNetwork
        * [Usage]: From a serialized object file, load the primitive
        * values of the stored object.
        *
        * @param path        Path of the serialized object file
        * @return            The network object with all its primitive values
        */
//       public Network deserializeNetwork(std::string path) {
//           Network net = nullptr;
//
//           try {
//               FileInputStream fileIn = new FileInputStream(path);
//               ObjectInputStream in = new ObjectInputStream(fileIn);
//               net = (Network) in.readObject();
//               in.close();
//               fileIn.close();
//           } catch (Exception i) {
//                i.printStackTrace();
//               return nullptr;
//           }
//           return net;
//       }
};

}

#endif

