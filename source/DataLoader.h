#ifndef DataLoader_h
#define DataLoader_h
#include <vector>
#include <string>
#include "Process.h"
using namespace std;

vector<Process> readInputCSV(const string& filename);
void writeOutputCSV(const string& filename, const string& algoName, const vector<Process>& processes);

#endif
