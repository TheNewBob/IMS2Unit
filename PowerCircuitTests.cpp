#include "stdafx.h"
#include "CppUnitTest.h"
#include "Common.h"
#include "TestUtils.h"
#include "PowerTypes.h"
#include "PowerChild.h"
#include "PowerParent.h"
#include "PowerConsumer.h"
#include "PowerBus.h"
#include "PowerSource.h"
#include "PowerCircuit.h"
#include "PowerCircuitManager.h"
#include "Calc.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IMS2Unit
{
	TEST_CLASS(PowerCircuitTest)
	{
	public:


		BEGIN_TEST_METHOD_ATTRIBUTE(Power_ForbiddenConnectionsTest)
			TEST_DESCRIPTION(L"Tests if forbidden connections can't be formed.")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(Power_ForbiddenConnectionsTest)
		{
			Logger::WriteMessage(L"\n\nTest: ForbiddenConnectionsTest\n");

			Logger::WriteMessage(L"Creating test assets\n");
			PowerCircuitManager *manager = new PowerCircuitManager();
			PowerConsumer *consumerlowvoltage = new PowerConsumer(5, 25, 100, 0);
			PowerConsumer *consumerhighvoltage = new PowerConsumer(27, 80, 100, 0);
			PowerBus *bus = new PowerBus(26, 1000, manager, 0);
			PowerSource *sourcelowvoltage = new PowerSource(5, 25, 200, 1, 0);
			PowerSource *sourcehighvoltage = new PowerSource(5, 25, 200, 1, 0);

			Logger::WriteMessage(L"Testing connection of invalid types\n");
			Assert::IsFalse(consumerlowvoltage->CanConnectToParent(sourcelowvoltage), L"consumer must NOT be able to connect to source!");
			Assert::IsFalse(sourcelowvoltage->CanConnectToChild(consumerlowvoltage), L"source must NOT be able to connect to consumer!");

			Logger::WriteMessage(L"Testing connection consumer - bus with incompatible voltages\n");
			Assert::IsFalse(consumerlowvoltage->CanConnectToParent(bus), L"consumer with lower voltage range must NOT able to connect to bus outside voltage range!");
			Assert::IsFalse(consumerhighvoltage->CanConnectToParent(bus), L"consumer with high voltage range must NOT be able to connect to bus outside voltage range!");
			Assert::IsFalse(bus->CanConnectToChild(consumerlowvoltage), L"bus above voltage range must NOT able to connect to consumer!");
			Assert::IsFalse(bus->CanConnectToChild(consumerhighvoltage), L"bus below voltage range must NOT able to connect to consumer!");

			Logger::WriteMessage(L"Testing connection source - bus with incompatible voltages\n");
			Assert::IsFalse(sourcelowvoltage->CanConnectToChild(bus), L"source with lower voltage range must NOT able to connect to bus outside voltage range!");
			Assert::IsFalse(sourcehighvoltage->CanConnectToChild(bus), L"source with high voltage range must NOT be able to connect to bus outside voltage range!");
			Assert::IsFalse(bus->CanConnectToParent(sourcelowvoltage), L"bus above voltage range must NOT able to connect to source!");
			Assert::IsFalse(bus->CanConnectToParent(sourcehighvoltage), L"bus below voltage range must NOT able to connect to source!");

			Logger::WriteMessage(L"Testing bus connections with self\n");
			Assert::IsFalse(bus->CanConnectToChild(bus), L"bus must NOT be able to be its own parent!");
			Assert::IsFalse(bus->CanConnectToParent(bus), L"bus must NOT be able to be its own child!");

			Logger::WriteMessage(L"Testing circular bus connections\n");
			PowerBus *bus2 = new PowerBus(26, 1000, manager, 0);
			PowerBus *bus3 = new PowerBus(26, 1000, manager, 0);
			bus2->ConnectChildToParent(bus);
			bus3->ConnectChildToParent(bus2);
			Assert::IsFalse(bus3->CanConnectToChild(bus), L"Circular bus connection must NOT be possible!");
			Assert::IsFalse(bus->CanConnectToChild(bus3), L"Circular bus connection must NOT be possible");
			Assert::IsFalse(bus3->CanConnectToParent(bus), L"Circular bus connection must NOT be possible!");
			Assert::IsFalse(bus->CanConnectToParent(bus3), L"Circular bus connection must NOT be possible");

			Logger::WriteMessage(L"Testing invalid multi-connections\n");
			PowerBus *bus4 = new PowerBus(26, 100, manager, 0);
			PowerSource *source = new PowerSource(20, 50, 200, 1, 0);
			bus4->ConnectChildToParent(source);
			Assert::IsFalse(bus->CanConnectToParent(source), L"Powersource must NOT allow connection to more than 1 bus!");
			Assert::IsFalse(source->CanConnectToChild(bus), L"Powersource must NOT allow connection to more than 1 bus!");

			PowerConsumer *consumer = new PowerConsumer(20, 30, 100, 0);
			consumer->ConnectChildToParent(bus4);
			Assert::IsFalse(consumer->CanConnectToParent(bus3), L"Consumer must NOT allow connection to more than 1 bus!");

			Logger::WriteMessage(L"cleaning up test assets\n");
			delete bus4;
			delete bus3;
			delete bus2;
			delete bus;
			delete source;
			delete consumerhighvoltage;
			delete consumerlowvoltage;
			delete sourcehighvoltage;
			delete sourcelowvoltage;
			delete consumer;
			delete manager;
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(Power_SimpleCircuitConnectionTest)
			TEST_DESCRIPTION(L"Tests connecting a bus, a consumer and a source.")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(Power_SimpleCircuitConnectionTest)
		{
			Logger::WriteMessage(L"\n\nTest: SimpleCircuitConnectionTest\n");

			Logger::WriteMessage(L"Creating test assets\n");
			PowerCircuitManager *manager = new PowerCircuitManager();
			PowerConsumer *consumer = new PowerConsumer(15, 30, 100, 0);
			PowerBus *bus = new PowerBus(26, 1000, manager, 0);
			PowerSource *source = new PowerSource(15, 30, 200, 1, 0);

			Logger::WriteMessage(L"Testing connection checks\n");
			Assert::IsTrue(bus->CanConnectToChild(consumer), TestUtils::Msg("Bus must be able to connect to consumer!"));
			Assert::IsTrue(bus->CanConnectToParent(source), TestUtils::Msg("Bus must be able to connect to source!"));
			Assert::IsTrue(source->CanConnectToChild(bus), TestUtils::Msg("source must be able to connect to bus!"));
			Assert::IsTrue(consumer->CanConnectToParent(bus), TestUtils::Msg("consumer must be able to connect to bus!"));

			source->ConnectParentToChild(bus);
			consumer->ConnectChildToParent(bus);

			Logger::WriteMessage(L"Testing circuit creation\n");
			Assert::IsTrue(manager->GetSize() == 1);

			Logger::WriteMessage(L"Testing circuit assignement\n");
			Assert::IsNotNull(source->GetCircuit(), TestUtils::Msg("source circuit is NULL!"));
			Assert::IsNotNull(bus->GetCircuit(), TestUtils::Msg("bus circuit is NULL!"));
			Assert::IsTrue(source->GetCircuit() == bus->GetCircuit(), TestUtils::Msg("bus and source are not in same circuit!"));

			vector<PowerChild*> checkchildren;
			source->GetChildren(checkchildren);
			Logger::WriteMessage(TestUtils::Msg("Number of children of source: " + Helpers::intToString(checkchildren.size()) + "\n"));
			Assert::IsTrue(checkchildren.size() == 1, TestUtils::Msg("source has wrong number of children!"));

			bus->GetChildren(checkchildren);
			Logger::WriteMessage(TestUtils::Msg("Number of children of bus: " + Helpers::intToString(checkchildren.size()) + "\n"));
			Assert::IsTrue(checkchildren.size() == 1, TestUtils::Msg("bus has wrong number of children!"));

			vector<PowerParent*> checkparents;
			consumer->GetParents(checkparents);
			Logger::WriteMessage(TestUtils::Msg("Number of parents of consumer: " + Helpers::intToString(checkparents.size()) + "\n"));
			Assert::IsTrue(checkparents.size() == 1, TestUtils::Msg("consumer has wrong number of parents!"));
			
			bus->GetParents(checkparents);
			Logger::WriteMessage(TestUtils::Msg("Number of parents of bus: " + Helpers::intToString(checkchildren.size()) + "\n"));
			Assert::IsTrue(checkchildren.size() == 1, TestUtils::Msg("bus has wrong number of children!"));
		
			Logger::WriteMessage(L"cleaning up test assets\n");
			delete consumer;
			delete source;
			delete bus;
			delete manager;
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(Power_SimpleCircuitEvaluationTest)
			TEST_DESCRIPTION(L"Tests evaluation of a simple circuit.")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(Power_SimpleCircuitEvaluationTest)
		{
			Logger::WriteMessage(L"\n\nTest: SimpleCircuitConnectionTest\n");

			Logger::WriteMessage(L"Creating test assets\n");
			PowerCircuitManager *manager = new PowerCircuitManager();
			PowerConsumer *consumer = new PowerConsumer(15, 30, 100, 0);
			PowerBus *bus = new PowerBus(26, 1000, manager, 0);
			PowerSource *source = new PowerSource(15, 30, 200, 1, 0);

			source->ConnectParentToChild(bus);
			consumer->ConnectChildToParent(bus);

			Logger::WriteMessage(L"Testing circuit evaluation at full load\n");
			consumer->SetConsumerLoad(1);
			manager->Evaluate();

			vector<PowerCircuit*> circuits;
			manager->GetPowerCircuits(circuits);
			PowerCircuit *circuit = circuits[0];

			Logger::WriteMessage(TestUtils::Msg("current input of consumer at current voltage: " + Helpers::doubleToString(consumer->GetInputCurrent()) + "\n"));
			Assert::IsTrue(consumer->GetInputCurrent() == 3.8461538461538461538461538461538, L"Consumer does not have correct input current!");

			Logger::WriteMessage(TestUtils::Msg("equivalent resistance of circuit: " + Helpers::doubleToString(circuit->GetEquivalentResistance()) + "\n"));
			Assert::IsTrue(circuit->GetEquivalentResistance() == 6.76, TestUtils::Msg("circuit has wrong equivalent resistance!"));
			Logger::WriteMessage(TestUtils::Msg("Total current in circuit: " + Helpers::doubleToString(circuit->GetCircuitCurrent()) + "\n"));
			Assert::IsTrue(circuit->GetCircuitCurrent() == 3.8461538461538461538461538461538, TestUtils::Msg("circuit has wrong current flow!"));

			Logger::WriteMessage(L"Testing powersource evaluation at full load\n");
			Logger::WriteMessage(TestUtils::Msg("Current voltage of powersource: " + Helpers::doubleToString(source->GetCurrentOutputVoltage()) + "\n"));
			Assert::IsTrue(source->GetCurrentOutputVoltage() == 26, L"Powersource does not have same voltage as bus!");
			Logger::WriteMessage(TestUtils::Msg("Maximum current output of powersource at current voltage: " + Helpers::doubleToString(source->GetMaxOutputCurrent()) + "\n"));
			Assert::IsTrue(source->GetMaxOutputCurrent() == 7.6923076923076923076923076923077, L"Powersource does not have correct maximum output current!");
			Logger::WriteMessage(TestUtils::Msg("Current current output of powersource: " + Helpers::doubleToString(source->GetOutputCurrent()) + "\n"));
			Assert::IsTrue(source->GetOutputCurrent() == circuit->GetCircuitCurrent(), L"Powersource does not have correct output current!");


			Logger::WriteMessage(L"Testing circuit evaluation at half load\n");
			consumer->SetConsumerLoad(0.5);
			circuit->Evaluate();

			Logger::WriteMessage(TestUtils::Msg("current input of consumer at current voltage: " + Helpers::doubleToString(consumer->GetInputCurrent()) + "\n"));
			Assert::IsTrue(consumer->GetInputCurrent() == 1.9230769230769230769230769230769, L"Consumer does not have correct input current!");

			Logger::WriteMessage(TestUtils::Msg("equivalent resistance of circuit: " + Helpers::doubleToString(circuit->GetEquivalentResistance()) + "\n"));
			Assert::IsTrue(circuit->GetEquivalentResistance() == 13.52, TestUtils::Msg("circuit has wrong equivalent resistance!"));
			Logger::WriteMessage(TestUtils::Msg("Total current in circuit: " + Helpers::doubleToString(circuit->GetCircuitCurrent()) + "\n"));
			Assert::IsTrue(circuit->GetCircuitCurrent() == 1.9230769230769230769230769230769, TestUtils::Msg("circuit has wrong current flow!"));

			Logger::WriteMessage(TestUtils::Msg("Current current output of powersource: " + Helpers::doubleToString(source->GetOutputCurrent()) + "\n"));
			Assert::IsTrue(source->GetOutputCurrent() == circuit->GetCircuitCurrent(), L"Powersource does not have correct maximum output current!");

			Logger::WriteMessage(L"cleaning up test assets\n");
			delete consumer;
			delete source;
			delete bus;
			delete circuit;
			delete manager;
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(Power_ComplexCircuitEvaluationTest)
			TEST_DESCRIPTION(L"Tests the evaluation of a complex, but isolated circuit.")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(Power_ComplexCircuitEvaluationTest)
		{
			/**
			The circuit tested here looks as follows
			S are sources, with id.
			BBBB are buses, with id.
			Plain numbers are consumers, the number is the id. Consumers are always written below the bus they are connected to.

			S1				  S2  S3
			|				  |   |
			BBBB1			  BBBBB2
			1 2 |			  | 5 |
				BBBB3	BBBBBB4   BBB5
				3 4 |   | 7 8     9  |
					BBBBB6			 BBBBB9
					| 6 |			 12  13
				BBBB7	BBB8
				| 10	 11
				S4

			*/

			Logger::WriteMessage(L"\n\nTest: ComplexCircuitConnectionTest\n");

			Logger::WriteMessage(L"Creating test assets\n");
			PowerCircuitManager *manager = new PowerCircuitManager();
			PowerConsumer *c1 = new PowerConsumer(8, 12, 10, 0);
			PowerConsumer *c2 = new PowerConsumer(8, 12, 20, 0);
			PowerConsumer *c3 = new PowerConsumer(8, 12, 10, 0);
			PowerConsumer *c4 = new PowerConsumer(8, 12, 30, 0);
			PowerConsumer *c5 = new PowerConsumer(8, 12, 20, 0);
			PowerConsumer *c6 = new PowerConsumer(8, 12, 50, 0);
			PowerConsumer *c7 = new PowerConsumer(8, 12, 30, 0);
			PowerConsumer *c8 = new PowerConsumer(8, 12, 50, 0);
			PowerConsumer *c9 = new PowerConsumer(8, 12, 60, 0);
			PowerConsumer *c10 = new PowerConsumer(8, 12, 30, 0);
			PowerConsumer *c11 = new PowerConsumer(8, 12, 20, 0);
			PowerConsumer *c12 = new PowerConsumer(8, 12, 10, 0);
			PowerConsumer *c13 = new PowerConsumer(8, 12, 40, 0);

			PowerSource *s1 = new PowerSource(8, 12, 200, 1, 0);
			PowerSource *s2 = new PowerSource(8, 12, 50, 4, 0);
			PowerSource *s3 = new PowerSource(8, 12, 50, 4, 0);
			PowerSource *s4 = new PowerSource(8, 12, 50, 4, 0);

			PowerBus *b1 = new PowerBus(10, 1000, manager, 0);
			PowerBus *b2 = new PowerBus(10, 1000, manager, 0);
			PowerBus *b3 = new PowerBus(10, 1000, manager, 0);
			PowerBus *b4 = new PowerBus(10, 1000, manager, 0);
			PowerBus *b5 = new PowerBus(10, 1000, manager, 0);
			PowerBus *b6 = new PowerBus(10, 1000, manager, 0);
			PowerBus *b7 = new PowerBus(10, 1000, manager, 0);
			PowerBus *b8 = new PowerBus(10, 1000, manager, 0);
			PowerBus *b9 = new PowerBus(10, 1000, manager, 0);

			Logger::WriteMessage(L"Testing circuit assembly\n");
			Assert::IsTrue(b1->CanConnectToParent(s1), L"b1 must be able to connect to s1\n");
			b1->ConnectChildToParent(s1);
			Assert::IsTrue(c1->CanConnectToParent(b1), L"c1 must be able to connect to b1\n");
			c1->ConnectChildToParent(b1);
			Assert::IsTrue(c2->CanConnectToParent(b1), L"c2 must be able to connect to b1\n");
			c2->ConnectChildToParent(b1);
			Assert::IsTrue(c3->CanConnectToParent(b3), L"c3 must be able to connect to b3\n");
			c3->ConnectChildToParent(b3);
			Assert::IsTrue(c4->CanConnectToParent(b3), L"c4 must be able to connect to b3\n");
			c4->ConnectChildToParent(b3);
			Assert::IsTrue(c5->CanConnectToParent(b2), L"c5 must be able to connect to b2\n");
			c5->ConnectChildToParent(b2);
			Assert::IsTrue(c6->CanConnectToParent(b6), L"c6 must be able to connect to b6\n");
			c6->ConnectChildToParent(b6);
			Assert::IsTrue(c7->CanConnectToParent(b4), L"c7 must be able to connect to b4\n");
			c7->ConnectChildToParent(b4);
			Assert::IsTrue(c8->CanConnectToParent(b4), L"c8 must be able to connect to b4\n");
			c8->ConnectChildToParent(b4);
			Assert::IsTrue(c9->CanConnectToParent(b5), L"c9 must be able to connect to b5\n");
			c9->ConnectChildToParent(b5);
			Assert::IsTrue(c10->CanConnectToParent(b7), L"c10 must be able to connect to b7\n");
			c10->ConnectChildToParent(b7);
			Assert::IsTrue(c11->CanConnectToParent(b8), L"c11 must be able to connect to b8\n");
			c11->ConnectChildToParent(b8);
			Assert::IsTrue(c12->CanConnectToParent(b9), L"c12 must be able to connect to b9\n");
			c12->ConnectChildToParent(b9);
			Assert::IsTrue(c13->CanConnectToParent(b9), L"c13 must be able to connect to b9\n");
			c13->ConnectChildToParent(b9);

			Assert::IsTrue(b1->CanConnectToChild(b3), L"b1 must be able to connect to b3\n");
			b1->ConnectParentToChild(b3);
			Assert::IsTrue(b2->CanConnectToChild(b5), L"b2 must be able to connect to b5\n");
			b2->ConnectParentToChild(b5);
			Assert::IsTrue(b2->CanConnectToChild(b4), L"b2 must be able to connect to b4\n");
			b2->ConnectParentToChild(b4);
			Assert::IsTrue(b3->CanConnectToChild(b6), L"b3 must be able to connect to b6\n");
			b3->ConnectParentToChild(b6);
			Assert::IsTrue(b4->CanConnectToChild(b6), L"b4 must be able to connect to b6\n");
			b4->ConnectParentToChild(b6);
			Assert::IsTrue(b5->CanConnectToChild(b9), L"b5 must be able to connect to b9\n");
			b5->ConnectParentToChild(b9);
			Assert::IsTrue(b6->CanConnectToChild(b7), L"b6 must be able to connect to b7\n");
			b6->ConnectParentToChild(b7);
			Assert::IsTrue(b6->CanConnectToChild(b8), L"b6 must be able to connect to b8\n");
			b6->ConnectParentToChild(b8);

			Assert::IsTrue(b2->CanConnectToParent(s2), L"b2 must be able to connect to s2\n");
			b2->ConnectChildToParent(s2);
			Assert::IsTrue(b2->CanConnectToParent(s3), L"b2 must be able to connect to s3\n");
			b2->ConnectChildToParent(s3);
			Assert::IsTrue(b7->CanConnectToParent(s4), L"b7 must be able to connect to s4\n");
			b7->ConnectChildToParent(s4);

			Assert::IsTrue(manager->GetSize() == 1, L"PowerCircuitManager should only have 1 circuit at this point!");


			Logger::WriteMessage(L"Setting consumer loads\n");
			//for a first test, we'll drive all consumers at 50%
			c1->SetConsumerLoad(0.5);
			c2->SetConsumerLoad(0.5);
			c3->SetConsumerLoad(0.5);
			c4->SetConsumerLoad(0.5);
			c5->SetConsumerLoad(0.5);
			c6->SetConsumerLoad(0.5);
			c7->SetConsumerLoad(0.5);
			c8->SetConsumerLoad(0.5);
			c9->SetConsumerLoad(0.5);
			c10->SetConsumerLoad(0.5);
			c11->SetConsumerLoad(0.5);
			c12->SetConsumerLoad(0.5);
			c13->SetConsumerLoad(0.5);

			Logger::WriteMessage(L"Testing circuit evaluation\n");
		
			manager->Evaluate();
			vector<PowerCircuit*> circuits;
			manager->GetPowerCircuits(circuits);
			PowerCircuit *circuit = circuits[0];

			Logger::WriteMessage(TestUtils::Msg("equivalent resistance of circuit: " + Helpers::doubleToString(circuit->GetEquivalentResistance()) + "\n"));
			Logger::WriteMessage(TestUtils::Msg("Total current in circuit: " + Helpers::doubleToString(circuit->GetCircuitCurrent()) + "\n"));

			Assert::IsTrue(circuit->GetCircuitCurrent() == 19, L"Circuit current is incorrect!");
			Assert::IsTrue(circuit->GetEquivalentResistance() == 0.52631578947368418, L"Equivalent resistance of circuit is incorrect!");


			Logger::WriteMessage(TestUtils::Msg("Current current output of S1: " + Helpers::doubleToString(s1->GetOutputCurrent()) + "\n"));
			Assert::IsTrue(s1->GetOutputCurrent() == 10.857142857142858, L"Power output of S1 is incorrect!");
			Logger::WriteMessage(TestUtils::Msg("Current current output of S2 " + Helpers::doubleToString(s2->GetOutputCurrent()) + "\n"));
			Assert::IsTrue(s2->GetOutputCurrent() == 2.7142857142857144, L"Power output of S2 is incorrect!");
			Logger::WriteMessage(TestUtils::Msg("Current current output of S3: " + Helpers::doubleToString(s3->GetOutputCurrent()) + "\n"));
			Assert::IsTrue(s3->GetOutputCurrent() == 2.7142857142857144, L"Power output of S3 is incorrect!");
			Logger::WriteMessage(TestUtils::Msg("Current current output of S4: " + Helpers::doubleToString(s4->GetOutputCurrent()) + "\n"));
			Assert::IsTrue(s4->GetOutputCurrent() == 2.7142857142857144, L"Power output of S4 is incorrect!");




			Logger::WriteMessage(L"Cleaning up test assets");


		}
	};
}