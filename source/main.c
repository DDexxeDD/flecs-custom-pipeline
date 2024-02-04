#include <stdio.h>

#include "flecs.h"

typedef struct
{
	unsigned int priority;
} PhasePriority;

/*
 * this is just the default flecs_entity_compare
 * 	but now we're testing on the priority component
 *
 * a lower priority value means a higher priority
 * 	0 is highest priority
 */
int priority_compare (ecs_entity_t e1, const void* pointer1, ecs_entity_t e2, const void* pointer2)
{
	unsigned int priority1 = ((PhasePriority*) pointer1)->priority;
	unsigned int priority2 = ((PhasePriority*) pointer2)->priority;

	return (priority1 > priority2) - (priority1 < priority2);
}

void print_system (ecs_iter_t* it)
{
	printf ("running: %s\n", ecs_get_name (it->world, it->system));
}

int main (void)
{
	printf ("---------------------------------------\n");
	printf ("pipeline with default system ordering:\n");
	printf ("---------------------------------------\n");

	/*
	 * ecs setup
	 */
	ecs_world_t* world = ecs_init ();

	/*
	 * this setup is pulled from:
	 * 	FlecsPipelineImport
	 * 		in: flecs/src/addons/pipeline/pipeline.c
	 */

	// the query finds entities:
	// 	which are a system
	// 	which are a phase, and breadth first sorts them by their dependence on phase
	//		whose dependees and parents are not disabled
	//
	//	this is the query for the default pipeline
	ecs_entity_t pipeline = ecs_pipeline(world,
	{
		.query =
		{
			.filter.terms =
			{
				{.id = EcsSystem},  // mandatory
				{.id = EcsPhase, .src.flags = EcsCascade, .src.trav = EcsDependsOn},
				{.id = EcsDisabled, .src.flags = EcsUp, .src.trav = EcsDependsOn, .oper = EcsNot},
				{.id = EcsDisabled, .src.flags = EcsUp, .src.trav = EcsChildOf, .oper = EcsNot}
			},
		}
	});

	ecs_set_pipeline (world, pipeline);

	/*
	 * the actual phases will depend on these anonymous phases
	 * 	this will allow the actual phases to run properly
	 * 		if one (or more) of the actual phases is disabled
	 */
	ecs_entity_t anonymous_phase0 = ecs_new (world, 0);
	ecs_entity_t anonymous_phase1 = ecs_new_w_pair (world, EcsDependsOn, anonymous_phase0);
	ecs_entity_t anonymous_phase2 = ecs_new_w_pair (world, EcsDependsOn, anonymous_phase1);
	ecs_entity_t anonymous_phase3 = ecs_new_w_pair (world, EcsDependsOn, anonymous_phase2);
	ecs_entity_t anonymous_phase4 = ecs_new_w_pair (world, EcsDependsOn, anonymous_phase3);

	ecs_entity_t phase0 = ecs_new_w_id (world, EcsPhase);
	ecs_entity_t phase1 = ecs_new_w_id (world, EcsPhase);
	ecs_entity_t phase2 = ecs_new_w_id (world, EcsPhase);
	ecs_entity_t phase3 = ecs_new_w_id (world, EcsPhase);
	ecs_entity_t phase4 = ecs_new_w_id (world, EcsPhase);

	ecs_add_pair (world, phase0, EcsDependsOn, anonymous_phase0);
	ecs_add_pair (world, phase1, EcsDependsOn, anonymous_phase1);
	ecs_add_pair (world, phase2, EcsDependsOn, anonymous_phase2);
	ecs_add_pair (world, phase3, EcsDependsOn, anonymous_phase3);
	ecs_add_pair (world, phase4, EcsDependsOn, anonymous_phase4);

	// this will replace phase2
	// 	just for show :)
	ecs_entity_t phase2_alternate = ecs_new_w_id (world, EcsPhase);
	ecs_add_pair (world, phase2_alternate, EcsDependsOn, anonymous_phase2);

	/*
	 * system execution order
	 * 	within a phase
	 * 		systems are sorted by their entity id
	 *
	 * 	if a system is created using a recycled entity id
	 * 		which is lower than a system that should come after it
	 * 			the system with the lower entity id will run first
	 *
	 * 	avoiding this will require the use of order_by in the pipeline query
	 * 		demonstrated later
	 */

	ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system 1a",
			.add = {ecs_dependson (phase1), phase1}
		}),
		.callback = print_system
	});

	ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system 2 regular",
			.add = {ecs_dependson (phase2), phase2}
		}),
		.callback = print_system
	});

	ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system 3",
			.add = {ecs_dependson (phase3), phase3}
		}),
		.callback = print_system
	});

	ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system 4",
			.add = {ecs_dependson (phase4), phase4}
		}),
		.callback = print_system
	});

	// adding this system to phase1 after the other systems
	// 	so we can see that it properly runs in phase1
	ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system 1b",
			.add = {ecs_dependson (phase1), phase1}
		}),
		.callback = print_system
	});

	ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system 2 alternate",
			.add = {ecs_dependson (phase2_alternate), phase2_alternate}
		}),
		.callback = print_system
	});

	printf ("\ndefault phases\n\n");

	// disable the alternate phase 2
	ecs_enable (world, phase2_alternate, false);

	ecs_progress (world, 0);

	printf ("\n\nalternate phase 2\n\n");

	// disable regular phase 2
	ecs_enable (world, phase2, false);
	// enable alternate phase 2
	ecs_enable (world, phase2_alternate, true);

	ecs_progress (world, 0);

	ecs_fini (world);

	/*
	 * pipeline with custom system ordering
	 */
	printf ("\n\n");
	printf ("---------------------------------------\n");
	printf ("pipeline with custom system order:\n");
	printf ("---------------------------------------\n");

	world = ecs_init ();

	ECS_COMPONENT (world, PhasePriority);

	pipeline = ecs_pipeline(world,
	{
		.query =
		{
			.filter.terms =
			{
				{.id = EcsSystem},  // mandatory
				{.id = EcsPhase, .src.flags = EcsCascade, .src.trav = EcsDependsOn},
				{.id = EcsDisabled, .src.flags = EcsUp, .src.trav = EcsDependsOn, .oper = EcsNot},
				{.id = EcsDisabled, .src.flags = EcsUp, .src.trav = EcsChildOf, .oper = EcsNot},
				{.id = ecs_id (PhasePriority)}
			},
			.order_by = priority_compare,
			.order_by_component = ecs_id (PhasePriority)
		}
	});

	ecs_set_pipeline (world, pipeline);

	anonymous_phase0 = ecs_new (world, 0);
	anonymous_phase1 = ecs_new_w_pair (world, EcsDependsOn, anonymous_phase0);

	phase0 = ecs_new_w_id (world, EcsPhase);
	phase1 = ecs_new_w_id (world, EcsPhase);

	ecs_add_pair (world, phase0, EcsDependsOn, anonymous_phase0);
	ecs_add_pair (world, phase1, EcsDependsOn, anonymous_phase1);

	/*
	 * add systems out of order
	 * 	so we can see them running in order
	 */
	ecs_entity_t system_c = ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system c",
			.add = {ecs_dependson (phase1), phase1, ecs_id(PhasePriority)}
		}),
		.callback = print_system
	});
	ecs_set (world, system_c, PhasePriority, {0});

	/*
	 * 'system a' and 'system b' will both be run in phase0
	 * we're adding 'system b' first
	 * 	but we want it to run _after_ 'system a'
	 * 	so we give 'system b' a lower priority
	 */
	ecs_entity_t system_b = ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system b",
			.add = {ecs_dependson (phase0), phase0}
		}),
		.callback = print_system
	});
	ecs_set (world, system_b, PhasePriority, {1});

	ecs_entity_t system_a = ecs_system (world,
	{
		.entity = ecs_entity (world,
		{
			.name = "system a",
			.add = {ecs_dependson (phase0), phase0, ecs_id(PhasePriority)}
		}),
		.callback = print_system
	});
	ecs_set (world, system_a, PhasePriority, {0});

	ecs_progress (world, 0);

	ecs_fini (world);

	return 0;
}
