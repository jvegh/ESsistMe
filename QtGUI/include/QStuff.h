/*! @brief Function prototypes for the ESsistMe tool.
 *
 *
 *  @author János Végh (jvegh)
 *  @bug No known bugs.
 */

#ifndef ESsistMe_QSTUFF_H
#define ESsistMe_QSTUFF_H
#include <QDir>
#include <QCoreApplication>
#include <fstream>      // std::ifstream
#include <stdlib.h>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::ostringstream
#include "Config.h"

using namespace std;

void
SetupSystemDirectories(QWidget* parent);

#endif //  ESsistMe_QSTUFF_H
