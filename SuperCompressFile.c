#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "General.h"
#include "FileHelper.h"
#include "SuperCompressFile.h"
#include "Product.h"
#include "myMacros.h"


int	saveSuperMarketToCompFile(const SuperMarket* pMarket, FILE* fp) {
	BYTE data[2] = { 0 };

	int len = (int)strlen(pMarket->name);
	data[0] = pMarket->productCount >> 2;
	data[1] = pMarket->productCount << 6 | len;
 		
    if (fwrite(&data, sizeof(BYTE), 2, fp) != 2) {
		CLOSE_RETURN_0(fp);
    }
    if (fwrite(pMarket->name, sizeof(char), len, fp) != len) {
		CLOSE_RETURN_0(fp);
    }
	//
   for (int i = 0; i < pMarket->productCount;i++) {
       if (!saveProductToCompFile(pMarket->productArr[i], fp)) {
		   CLOSE_RETURN_0(fp);
       }
	}
	return 1;
}

int	loadSuperMarketFromCompFile(SuperMarket* pMarket, FILE* fp) 
{
	BYTE data[2];
	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		CLOSE_RETURN_0(fp);

	pMarket->productCount =  data[0] <<2  | data[1] >> 6 ;
	
	int len = data[1] & 0x3f;

	pMarket->name = (char*)calloc(len + 1, sizeof(char));
	if (!pMarket->name)
		return 0;

	if (fread(pMarket->name, sizeof(char), len, fp) != len){
		free(pMarket->name);
		CLOSE_RETURN_0(fp);
	}
    //
	pMarket->productArr = (Product**)malloc(sizeof(Product*) * pMarket->productCount); 
    if (!pMarket->productArr) {
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
    }
	for (int i = 0; i < pMarket->productCount; i++) {
		pMarket->productArr[i] = (Product*)malloc(sizeof(Product));
        if (!pMarket->productArr[i]) {
			freeProducts(pMarket->productArr);
			FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
        }
        if (!loadProductFromCompFile(pMarket->productArr[i], fp)) {
            freeProducts(pMarket->productArr);
			FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp);
        }
	}
	return 1;
}

int saveNameAndBarcode(const Product* pProduct, FILE* fp) {

	BYTE data[4] = { 0 };
	
	int len = (int)strlen(pProduct->name);
	eProductType productType = -1;
	if (pProduct->barcode[0] == 'S') productType = 3;
	else {
		if (pProduct->barcode[1] == 'V')  productType = 0 ;
		if (pProduct->barcode[1] == 'R')  productType = 1;
		if (pProduct->barcode[1] == 'Z')  productType = 2;

	}
	data[0] = (pProduct->barcode[2] - '0') << 4 | (pProduct->barcode[3] - '0');
	data[1] = (pProduct->barcode[4] - '0') << 4 | (pProduct->barcode[5] - '0');
	data[2] = (pProduct->barcode[6] - '0') << 4 | productType << 2 | len >> 2; 
	data[3] = len << 6;

	if (fwrite(&data, sizeof(BYTE), 4, fp) != 4)
		return 0;
	if (fwrite(pProduct->name, sizeof(char), len, fp) != len)
		return 0;

	return 1;
}

int	loadNameAndBarcode(Product* pProduct, FILE* fp){
	BYTE data[4];
	if (fread(&data, sizeof(BYTE), 4, fp) != 4) return 0;
	pProduct->type = (data[2] >> 2) & 0x3;
	char* prefix = getProductTypePrefix(pProduct->type);
	int number = ((data[0] >> 4)) * 10000;
	number += (data[0] & 0xF) * 1000;
	number += ((data[1] >> 4) & 0xF) * 100;
	number += (data[1] & 0xF) * 10;
	number += (data[2] >> 4) ;

	sprintf(pProduct->barcode, "%s%05d", prefix, number);

	int len = ((data[2] & 0x3) << 2) | ((data[3] >> 6) & 0xf);
	char* name = (char*)calloc(len+1, sizeof(char));
	if (!name) return 0;
    if (fread(name, sizeof(char), len, fp) != len) {
        free(name);
        return 0;
    }
	strcpy(pProduct->name, name);
	free(name);
	return 1;
}

int	saveCountAndPrice(const Product* pProduct, FILE* fp) {
	BYTE data[3] = { 0 };

	data[0] = pProduct->count;
	int agorot = ((int)(pProduct->price * 100)) % 100;

	data[1] = agorot << 1 | (int)(pProduct->price) >> 8;
	data[2] = (int)(pProduct->price);

	if (fwrite(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;

	return 1;
}

int	loadCountAndPrice(Product* pProduct, FILE* fp) {
	BYTE data[3];
	if (fread(&data, sizeof(BYTE), 3, fp) != 3)
		return 0;

	pProduct->count = data[0];
	float agorot = (data[1] >> 1) & 0x7f;
	int shekels = (data[1] & 0x1) << 8 | data[2] & 0xff;
	pProduct->price = shekels + (agorot / 100); 
	return 1;
}

int	saveDate(const Product* pProduct, FILE* fp) {
	BYTE data[2] = { 0 };

	Date date = pProduct->expiryDate;
	
	data[0] = date.day << 3 | date.month >> 1;
	data[1] = date.month << 7 | (date.year - 2024) << 4;
	
	if (fwrite(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	return 1;
}

int	loadDate( Product* pProduct, FILE* fp) {

	BYTE data[2];
	if (fread(&data, sizeof(BYTE), 2, fp) != 2)
		return 0;

	Date expiryDate;
	expiryDate.day = (data[0] >> 3) & 0x1f;
    expiryDate.month = (data[0] & 0x7) << 1 | (data[1] >> 7) & 0x1;
	expiryDate.year = ((data[1] >> 4) & 0x7) + 2024;
	pProduct->expiryDate = expiryDate;

	return 1;
}

int saveProductToCompFile(const Product* pProduct, FILE* fp) {
	if (!saveNameAndBarcode(pProduct, fp)) return 0;
	if (!saveCountAndPrice(pProduct, fp)) return 0;
	if (!saveDate(pProduct, fp)) return 0;
	return 1;
}


int loadProductFromCompFile(Product* pProduct, FILE* fp) {
	if (!loadNameAndBarcode(pProduct, fp)) return 0;
	if (!loadCountAndPrice(pProduct, fp)) return 0;
	if (!loadDate(pProduct, fp)) return 0;
	return 1;
}

