#ifndef REALGA_MESSAGES_H
#define REALGA_MESSAGES_H

#define REALGA_ERROR(condition, msg) {if(condition) {cerr << "ERROR: " << __func__<< " \""<< msg<<"\" "<< " File: "  << __FILE__<< " Line:" << __LINE__<< endl; exit(-1);}}

#endif // REALGA_MESSAGES_H