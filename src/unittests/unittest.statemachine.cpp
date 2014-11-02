#include "gtest/gtest.h"

#include "engine.core/sys.h"
#include "engine.core/state_machine.h"
#include "engine.core/event_handler.h"

class state_SettingsMenu;

class state_StartMenu: public state
{
public:
	virtual void OnEntry() 
	{
		std::cout << "state_StartMenu::OnEntry" << std::endl;
		timeInState = 0.f;
	}

	virtual void OnExit() 
	{
		std::cout << "state_StartMenu::OnExit" << std::endl;
	}

	virtual Status EvaluateTransitions(float deltaTime, std::shared_ptr<state>& outNextState) 
	{		
		timeInState += deltaTime;
		if ( timeInState >= 5.f )
		{
			outNextState = std::shared_ptr<state>( state_factory::Create<state_SettingsMenu>() );
			return Done;
		}
		
		return state::EvaluateTransitions(deltaTime, outNextState);
	}

	float timeInState;
};

class state_SettingsMenu : public state
{
public:
	virtual void OnEntry() 
	{
		std::cout << "state_SettingsMenu::OnEntry" << std::endl;
		m_status = Processing;
		m_onExit += [&](void*, int) { m_status = Done; };
	}

	virtual void OnExit() 
	{
		std::cout << "state_SettingsMenu::OnExit" << std::endl;
	}

	virtual Status EvaluateTransitions(float deltaTime, std::shared_ptr<state>& outNextState) 
	{		
		UNREFERENCED(deltaTime);
		outNextState = nullptr;

		

		return m_status;
		//return state::EvaluateTransitions(deltaTime, outNextState);
	}
	Status m_status;
	event_handler<int> m_onExit;
};

class StateMachineTest : public testing::Test
{
public:

};



TEST_F(StateMachineTest, Default)
{
	state_machine stateMachine;
	stateMachine.m_currentState = std::make_shared<state_StartMenu>( state_StartMenu() );
	
	float time = 0.f;

	while ( time <= 1000.f )
	{
		if (stateMachine.Update(time) == state_machine::Done)
		{
			break;
		} 

		if ( time >= 2.f )
		{
			
		}

		time += 1.f / 30.f;
	}
	

}