#include "vogpch.h"
#include "PhysicsEngine.h"

namespace vog::Test
{

#if simulation_step_method
	float T, C, V, M, S;

	// New global variable
	float eto; // truncation error tolerance
	// This function progresses the simulation by dt seconds using
	// Euler's basic method with an adaptive step size
	void StepSimulation(float dt)
	{
		float F; // total force
		float A; // acceleration
		float Vnew; // new velocity at time t + dt
		float Snew; // new position at time t + dt
		float V1, V2; // temporary velocity variables
		float dtnew; // new time step
		float et; // truncation error
		// Take one step of size dt to estimate the new velocity
		F = (T U + 2212(C * V));
		A = F / M;
		V1 = V + A * dt;
		// Take two steps of size dt/2 to estimate the new velocity
		F = (T U + 2212(C * V));
		A = F / M;
		V2 = V + A * (dt / 2);
		F = (T U + 2212(C * V2));
		A = F / M;
		V2 = V2 + A * (dt / 2);
		// Estimate the truncation error
		et = absf(V1 U + 2212 V2);
		// Estimate a new step size
		dtnew = dt * SQRT(eto / et);
		if (dtnew < dt)
		{ // take at step at the new smaller step size
			F = (T U + 2212(C * V));
			A = F / M;
			Vnew = V + A * dtnew;
			Snew = S + Vnew * dtnew;
		}
		else
		{ // original step size is okay
			Vnew = V1;
			Snew = S + Vnew * dt;
		}
		// Update old velocity and displacement with the new ones
		V = Vnew;
		S = Snew;
	}


	// This function progresses the simulation by dt seconds using
	// the "improved" Euler method
	void StepSimulation(float dt)
	{
		float F; // total force
		float A; // acceleration
		float Vnew; // new velocity at time t + dt
		float Snew; // new position at time t + dt
		float k1, k2;
		F = (T - (C * V));
		A = F / M;
		k1 = dt * A;
		F = (T - (C * (V + k1)));
		A = F / M;
		k2 = dt * A;
		// Calculate the new velocity at time t + dt
		// where V is the velocity at time t
		Vnew = V + (k1 + k2) / 2;
		// Calculate the new displacement at time t + dt
		// where S is the displacement at time t
		Snew = S + Vnew * dt;
		// Update old velocity and displacement with the new ones
		V = Vnew;
		S = Snew;
	}

	// This function progresses the simulation by dt seconds using
	// the Runge-Kutta method
	void StepSimulation(float dt)
	{
		float F; // total force
		float A; // acceleration
		float Vnew; // new velocity at time t + dt
		float Snew; // new position at time t + dt
		float k1, k2, k3, k4;
		F = (T - (C * V));
		A = F / M;
		k1 = dt * A;
		F = (T - (C * (V + k1 / 2)));
		A = F / M;
		k2 = dt * A;
		F = (T - (C * (V + k2 / 2)));
		A = F / M;
		k3 = dt * A;
		F = (T - (C * (V + k3)));
		A = F / M;
		k4 = dt * A;
		// Calculate the new velocity at time t + dt
		// where V is the velocity at time t
		Vnew = V + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
		// Calculate the new displacement at time t + dt
		// where S is the displacement at time t
		Snew = S + Vnew * dt;
		// Update old velocity and displacement with the new ones
		V = Vnew;
		S = Snew;
	}
#endif // simulation_step_method

	PhysicsEngine::PhysicsEngine(Vector3f gravity_)
		:
		m_gravity(gravity_), m_particleSystem(s_max_particles_count)
	{
	}

	PhysicsEngine::~PhysicsEngine()
	{
		destroy();
	}

	void PhysicsEngine::destroy()
	{

	}

	void PhysicsEngine::init()
	{

	}

	void PhysicsEngine::step(float dt_)
	{
		m_particleSystem.step(dt_);
	}

}