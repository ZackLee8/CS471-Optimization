// Zack Lee
// 03/09/2026
// RunExperiments.h

#ifndef RUN_EXPERIMENTS_H
#define RUN_EXPERIMENTS_H

#include "ACO.h"
#include "NEH.h"
#include "core.h"

#include <string>
#include <vector>


class RunExperiments {
public:
    explicit RunExperiments(const std::string& dataDirectory = "Taillard_TestData");

    std::vector<result> runAll() const;

private:
    std::string dataDirectory;
    NEH nehSolver;
    ACO acoSolver;

    std::string resolveDataDirectory() const;
    std::string resolveOutputDirectory(const std::string& resolvedDataDirectory) const;
    std::vector<result> runInstance(const std::string& instancePath) const;
    void printResultSummary(const result& output) const;
    void writeCombinationCSVs(const std::vector<result>& allResults,
                              const std::string& outputDirectory) const;
    std::string sequenceToString(const std::vector<int>& sequence) const;
    std::string buildResultTag(const result& output) const;
};

#endif // RUN_EXPERIMENTS_H
