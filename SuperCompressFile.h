#pragma once
#include <stdio.h>
#include "Supermarket.h"


typedef unsigned char BYTE;

int				saveSuperMarketToCompFile(const SuperMarket* pMarket, FILE* fp);
int				loadSuperMarketFromCompFile(SuperMarket* pMarket, FILE* fp);

int				saveNameAndBarcode(const Product* pProduct, FILE* fp);
int				loadNameAndBarcode(Product* pProduct, FILE* fp);

int				saveCountAndPrice(const Product* pProduct, FILE* fp);
int				loadCountAndPrice(Product* pProduct, FILE* fp);

int				saveDate(const Product* pProduct, FILE* fp);
int				loadDate(Product* pProduct, FILE* fp);

int				saveProductToCompFile(const Product* pProduct, FILE* fp);
int				loadProductFromCompFile(const Product* pProduct, FILE* fp);

