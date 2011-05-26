#ifndef __DATEPOINT_H__
#define __DATEPOINT_H__

class DatePoint {
	public:
		DatePoint();
		~DatePoint();
		
		void clear();
		void print();
		
		long getValue();
		int getYear();
		unsigned char getMonth();
		unsigned char getDay();
		
		void setYear(int _year);
		void setMonth(unsigned char _month);
		void setDay(unsigned char _day);

		char* getString();
		void fromString(char *str);
		
	protected:
		long value;
		int year;
		unsigned char month;
		unsigned char day;
		char string[32];

		void normalize();
		void buildString();
};

#endif

