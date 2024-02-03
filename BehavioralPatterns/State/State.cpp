#include <iostream>
#include <string>
#include <vector>
// Back-end
#include <boost/msm/back/state_machine.hpp>
// Front-end
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

std::vector<std::string> state_names {
  "off hook",
  "connecting",
  "connected",
  "on hold",
  "destroyed"
};

// Transitions/events
struct CallDialed {};
struct HungUp {};
struct CallConnected {};
struct PlacedOnHold {};
struct TakenOffHold {};
struct LeftMessage {};
struct PhoneThrownIntoWall {};

struct PhoneStateMachine : boost::msm::front::state_machine_def<PhoneStateMachine>
{
    bool angry{ true }; // Start with false

    struct OffHook : boost::msm::front::state<> {};
    struct Connecting : boost::msm::front::state<>
    {
        template <class Event, class FSM>
        void on_entry(Event const& evt, FSM&)
        {
            std::cout << "We are connecting..." << std::endl;
        }
        // also on_exit
    };
    struct Connected : boost::msm::front::state<> {};
    struct OnHold : boost::msm::front::state<> {};
    struct PhoneDestroyed : boost::msm::front::state<> {};

    struct PhoneBeingDestroyed
    {
        template <class EVT, class FSM, class SourceState, class TargetState>
        void operator()(EVT const&, FSM&, SourceState&, TargetState&)
        {
            std::cout << "Phone breaks into a million pieces" << std::endl;
        }
    };

    struct CanDestroyPhone
    {
        template <class EVT, class FSM, class SourceState, class TargetState>
        bool operator()(EVT const&, FSM& fsm, SourceState&, TargetState&)
        {
            return fsm.angry;
        }
    };

    // Start, event, target, action, guard
    struct transition_table : boost::mpl::vector <
        boost::msm::front::Row<OffHook, CallDialed, Connecting>,
        boost::msm::front::Row<Connecting, CallConnected, Connected>,
        boost::msm::front::Row<Connected, PlacedOnHold, OnHold>,
        boost::msm::front::Row<OnHold, PhoneThrownIntoWall, PhoneDestroyed,
        PhoneBeingDestroyed, CanDestroyPhone>
    > {};

    // Starting state
    typedef OffHook initial_state;

    // What happens if there's nowhere to go
    template <class FSM, class Event>
    void no_transition(Event const& e, FSM&, int state)
    {
        std::cout << "No transition from state " << state_names[state]
            << " on event " << typeid(e).name() << std::endl;
    }
};

int main()
{
    boost::msm::back::state_machine<PhoneStateMachine> phone;

    auto info = [&]()
        {
            auto i = phone.current_state()[0];
            std::cout << "The phone is currently " <<
                state_names[i] << std::endl;
        };

    info();
    phone.process_event(CallDialed{});
    info();
    phone.process_event(CallConnected{});
    info();
    phone.process_event(PlacedOnHold{});
    info();
    phone.process_event(PhoneThrownIntoWall{});
    info();

    // Try process_event here :)
    phone.process_event(CallDialed{});

    std::cout << "We are done using the phone" << std::endl;

    return 0;
}
