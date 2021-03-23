#pragma once
class Box
{
private:
  
	int minRow;
	int minCol;
	int maxRow;
	int maxCol;
  

public:
  
	Box();

	Box(int, int, int, int);

	~Box();

	int getMinRow();

	void setMinRow(int);

	int getMinCol();

	void setMinCol(int);

	int getMaxRow();

	void setMaxRow(int);

	int getMaxCol();

	void setMaxCol(int);
};
