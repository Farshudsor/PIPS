#ifndef PIPSSINTERFACE_HPP
#define PIPSSINTERFACE_HPP

#include "BALPSolverBase.hpp"
#include "stochasticInput.hpp"
#include "BALPSolverInterface.hpp"

#include "PIPSLogging.hpp"
#include "CoinShallowPackedVector.hpp"
class BALPSolverDual;

// public interface to the solver
// handles switching between primal/dual solvers to clean up infeasibilities
class PIPSSInterface : public BALPSolverInterface<PIPSSInterface> {
public:
	PIPSSInterface(stochasticInput &in, BAContext &ctx, solveType t);
	PIPSSInterface(const BAData &_d, solveType t);
	~PIPSSInterface();


	// if writeBasisOnly is false, we also write edge weights and perturbed objective/lb/ub's.
	// this is for resuming in the middle of a solve
	void writeStatus(const std::string &filebase, bool appendIterateNumber = false,
			 bool writeBasisOnly = true) { solver->writeStatus(filebase,appendIterateNumber,writeBasisOnly); }
	void loadStatus(const std::string &filebase) { solver->loadStatus(filebase); }

	void go();

	void setPrimalTolerance(double val) { solver->setPrimalTolerance(val); }
	void setDualTolerance(double val) { solver->setDualTolerance(val); }
	double getObjective() const { return solver->objval; }
	solverState getStatus() const { return solver->status; }

	std::vector<double> getFirstStagePrimalColSolution() const;
	std::vector<double> getSecondStagePrimalColSolution(int scen) const;
	std::vector<double> getFirstStageDualColSolution() const;
	std::vector<double> getSecondStageDualColSolution(int scen) const;
	// these are the multipliers on the rows, not the reduced costs for
	// the slacks corresponding to the rows. We need to reconcile this with Clp.
	std::vector<double> getSecondStageDualRowSolution(int scen) const;

        const denseBAVector& getPrimalSolution() const { return solver->getPrimalSolution(); }

	void setFirstStageColState(int idx,variableState s);
	void setFirstStageRowState(int idx,variableState);
	void setSecondStageColState(int scen, int idx,variableState);
	void setSecondStageRowState(int scen, int idx,variableState);
	void commitStates();

	variableState getFirstStageColState(int idx) const;
	variableState getFirstStageRowState(int idx) const;
	variableState getSecondStageColState(int scen, int idx) const;
	variableState getSecondStageRowState(int scen, int idx) const;

	int getNumIterations() const { return solver->nIter; }

	// override default
	void setStates(const BAFlagVector<variableState> &s) { solver->setStates(s); }

	// will dump current status every d iterations. zero to disable.
	void setDumpFrequency(int d, const std::string &outputname) { solver->setDumpFrequency(d,outputname); }

	void setFirstStageColLB(int idx, double newLb);
	void setFirstStageColUB(int idx, double newUb);
        void setSecondStageColLB(int scen, int idx, double newLb);
        void setSecondStageColUB(int scen, int idx, double newUb);

        void setLB(const denseBAVector& lb);
        void setUB(const denseBAVector& ub);

        const denseBAVector& getLB() const { return solver->myLB(); }
        const denseBAVector& getUB() const { return solver->myUB(); }

        const denseBAVector& getVarObjective() const { return d.c; }
        const BAFlagVector<constraintType>& getVariableTypes() const {return d.vartype;}
        const BAData& getBAData() const { return d; }
        solveType getSolveType() const { return st; }

	// add row that preserves block-angular structure
	// e.g. lb <= elts1*x + elts2*y_i <= ub
	void addRow(const std::vector<double>& elts1, const std::vector<double> &elts2, int scen, double lb = -COIN_DBL_MAX, double ub = COIN_DBL_MAX);
	void commitNewRows();

	double primalError() { return solver->calculateLargestPrimalError(); }

	static bool isDistributed() { return true; }

	const CoinShallowPackedVector retrieveARow(int index) const;

	const CoinShallowPackedVector retrieveWRow(int index,int scen) const;

	const CoinShallowPackedVector retrieveTRow(int index,int scen) const;

	const CoinShallowPackedVector retrieveACol(int index) const;

	const CoinShallowPackedVector retrieveWCol(int index,int scen) const;

	const CoinShallowPackedVector retrieveTCol (int index,int scen) const;


	int isRowFeasible(int index, int scen,denseBAVector &solution);

	void commitNewColsAndRows();

	void generateBetas(sparseBAVector &beta);

	void generateNonBasicRow(BAIndex in, sparseBAVector &row);

	void addFirstStageRow(const std::vector<double>& elts1, double lb = -COIN_DBL_MAX, double ub = COIN_DBL_MAX);

	void addSecondStageRows(const std::vector< std::vector <double> >& elts1, const std::vector< std::vector <double> >&elts2, int scen, std::vector<double> &lb, std::vector<double> &ub, int nRows);

	void addFirstStageRows(const std::vector< std::vector <double> >& elts, std::vector<double> &lb, std::vector<double> &ub, int nRows);

	int addFirstStageColumn(double lb, double ub, double c);

	int addSecondStageColumn(int scen,double lb, double ub, double cobj);

	void deleteLastFirstStageConsecutiveRows(int nRows);

	void deleteLastSecondStageConsecutiveRows(int scenario, int nRows);

	void deleteLastFirstStageConsecutiveColumns(int nCols);

	void deleteLastSecondStageConsecutiveColumns(int scenario, int nCols);


	const BADimensionsSlacks& getSlackDims() const { return d.dims; }


protected:
	void setPhase1() { solver->phase1 = true; boundsChanged = true; }
	const BAFlagVector<variableState>& getStatesRef() const { return solver->getStates(); }
	const BADimensions& getDims() const { return d.dims.inner; }

	BALPSolverBase *solver;

	bool boundsChanged;
        solveType st;
	BAData d;

friend class BALPSolverDual;

};


#endif
