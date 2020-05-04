/* 
 * esmini - Environment Simulator Minimalistic 
 * https://github.com/esmini/esmini
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 * 
 * Copyright (c) partners of Simulation Scenarios
 * https://sites.google.com/view/simulationscenarios
 */

 /*
  * This application is a minimalistic example of how to make use of the scenarioengine DLL to simply play and view OpenSCENARIO files
  * In addition to Init and Step, it shows how to retrieve the state of scenario objects.
  */

#include "stdio.h"
#include "scenarioenginedll.hpp"
#include "CommonMini.hpp"


#define MAX_N_OBJECTS 10
#define TIME_STEP 0.017f
#define DURATION 25
#define MAX_DETECTIONS 8

static SE_ScenarioObjectState states[MAX_N_OBJECTS];

void log_callback(const char *str)
{
	printf("%s\n", str);
}

int main(int argc, char *argv[])
{
	// Use logger callback
	Logger::Inst().SetCallback(log_callback);

	if (argc < 2)
	{
		LOG("Usage: %s <osc filename>\n", argv[0]);
		return -1;
	}

	for (int a = 0; a < 1; a++)
	{

		if (SE_Init(argv[1], 0, 1, 1, 0, 2.0f) != 0)
		{
			LOG("Failed to load %s", argv[1]);
			return -1;
		}

		// Add four sensors around the vehicle
		SE_AddObjectSensor(0, 4.0f, 0.0f, 0.5f, 0.0f, 6.0f, 50.0f, (float)(50.0 * M_PI / 180.0), MAX_DETECTIONS);
		SE_AddObjectSensor(0, 2.0f, 1.0f, 0.5f, 1.5f, 1.0f, 20.0f, (float)(120.0 * M_PI / 180.0), MAX_DETECTIONS);
		SE_AddObjectSensor(0, 2.0f, -1.0f, 0.5f, -1.5f, 1.0f, 20.0f, (float)(120.0 * M_PI / 180.0), MAX_DETECTIONS);
		SE_AddObjectSensor(0, -1.0f, 0.0f, 0.5f, 3.14f, 5.0f, 30.0f, (float)(50.0 * M_PI / 180.0), MAX_DETECTIONS);

		for (int i = 0; i*TIME_STEP < DURATION; i++)
		{
			if (SE_StepDT(TIME_STEP) != 0)
			{
				return 0;
			}

#if 1  // set to 1 to demonstrate how to query sensors - only first one in this case

			printf("Detections [sensor ID, Object ids]:");
			int objList[MAX_DETECTIONS];  // make room for max nr vehicles, as specified when added sensor
			for (int j = 0; j < 4; j++)
			{
				int nHits = SE_FetchSensorObjectList(j, objList);
				for (int k = 0; k < nHits; k++)
				{
					printf(" [%d, %d]", j, objList[k]);
				}
			}
			printf("\n");
#endif

			if (i == (int)(0.5*DURATION / TIME_STEP))
			{
				// Halfway through, pause the simulation for a few seconds
				// to demonstrate how camera can still move independently
				// when running viewer in a separate thread
				LOG("Taking a 4 sec nap - if running with threads you can move camera around meanwhile");
				SE_sleep(4000);
			}
			else
			{
				// Normal case, sleep until its time for next simulation step
				SE_sleep((unsigned int)(TIME_STEP * 1000));
			}
		}

		SE_Close();
	}

	return 0;
}
