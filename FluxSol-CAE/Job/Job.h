#ifndef _JOB_H_
#define _JOB_H_

class Job
{

	CFDModel *model;	//INCLUDES INHERITED CFDMODELS, SIMPLE, THERMAL, AND SO ON
	
	public:
		Job{}
		Model * Model(){return model;}
		RunIteration(){}
		virtual void Submit(){}
		
};

#endif