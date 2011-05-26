#ifndef __DATEPOINT_CC__
#define __DATEPOINT_CC__

#include <stdio.h>
#include <string.h>
#include "DatePoint.h"

DatePoint::DatePoint() { }

DatePoint::~DatePoint() { }

unsigned char DatePoint::getDay() { return day; }
unsigned char DatePoint::getMonth() { return month; }
int DatePoint::getYear() { return year; }
long DatePoint::getValue() { return value; }
char* DatePoint::getString() { return string; }

void DatePoint::normalize() {
	
	// normalize positive-value
	// if (positive < 0) positive = -1; else positive = 1;
	
	// limit month and day values
	if (day > 31) day = 31;
	if (month > 12) month = 12;
	
	// maybe later as an option:
	// normalize unspecified datepoints
	if (month == 0) day = 0;

	// calculate absolute value
	value = (year*10000) + (month*100) + day;
}

void DatePoint::print() {
	printf("\nYear:\t%i\n",year);
	printf("Month:\t%i\n",month);
	printf("Day:\t%i\n",day);
	printf("Value:\t%li\n",value);
	printf("String:\t%s\n\n",string);
}

void DatePoint::buildString() {
	int c = 0;
	
	string[0] = 0; 
	
	sprintf(string,"%i", year);
	c = strlen(string);
	if (month) {
		sprintf(string+c," / %s%i",(month<10?"0":""),month);
		c = strlen(string);
		if (day) {
			sprintf(string+c," / %s%i",(day<10?"0":""), day);
			// c = strlen(string); // save energy
		}
	}
}

void DatePoint::clear() {
	year = 0;
	month = 0;
	day = 0;
	value = 0;
	string[0] = 0;
}

void DatePoint::fromString(char *str) {
	int c = 0, len = 0, stage = 0;
	int *cur_num = NULL;
	int positive = 1;
	
	clear();
	len = strlen(str);
	cur_num = &year;
	
	if (str[0]=='-') {
		c = 1;
		positive = -1;
	}
	for (;c<len;c++) {
		if (str[c]=='/') {
			stage++;
			if (stage==1) cur_num = (int*) &month;
			if (stage==2) cur_num = (int*) &day;
			continue;
		}
		if ((str[c]>='0') && (str[c]<='9')) {
			*cur_num = (*cur_num * 10) + (str[c]-'0');
		}
	}
	if (!positive) year = -year;
	normalize();
	buildString();
}

void DatePoint::setYear(int _year) {
	year = _year;
	normalize();
	buildString();
}

void DatePoint::setMonth(unsigned char _month) {
	month = _month;
	normalize();
	buildString();
}

void DatePoint::setDay(unsigned char _day) {
	day = _day;
	normalize();
	buildString();
}

#endif

