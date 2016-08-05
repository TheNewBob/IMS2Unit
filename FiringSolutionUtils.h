#pragma once

/**
 * Contains static helper functions to expedite tests of firing solutions
 */
class FiringSolutionUtils
{
public:
	FiringSolutionUtils();
	~FiringSolutionUtils();

	static string ThGroupToString(THGROUP_TYPE group);

	/**
	* \brief creates a quintet of rcs thrusters
	*
	* On a quintet with dir 0,0,1 and rot 0,1,0, the directions of thrust are aligned as follows:
	* thruster[0] 0,0,1
	* thruster[1] 1,0,0
	* thruster[2] -1,0,0
	* thruster[3] 0,1,0
	* thruster[4] 0,-1,0
	* \param pos The shared attack point of all five thrusters
	* \param dir The direction of the first thruster in the quintet (the only without a mirror)
	* \param rot The rotation of the thruster quintet (not of individual thrusters, for which rotation has no meaning)
	*/
	static vector<FiringSolutionThruster> CreateRcsQuintet(VECTOR3 pos, VECTOR3 dir, VECTOR3 rot);

	static bool IsThrusterInGroup(FiringSolutionThruster thruster, THGROUP_TYPE group);

	static bool IsThrusterInThisManyGroups(FiringSolutionThruster thruster, UINT n);

	static void LogThruster(FiringSolutionThruster thruster);

};

