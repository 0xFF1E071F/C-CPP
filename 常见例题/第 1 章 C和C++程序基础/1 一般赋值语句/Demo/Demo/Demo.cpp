// Demo.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

int main()
{
	int x = 3, y, z;

	x *= (y = z = 4);printf("x = %d\n", x); //12
	/**
	 * z = 4;
	 * y = z;
	 * x *= 4; 
	 * x = x*4 = 3 * 4 = 12;
	 */
	

	z = 2;

	x = (y = z); printf("x = %d\n", x); //2
	/**
	 * z =2;
	 * y = z;
	 * x = y
	 */

	x = (y == z); printf("x = %d\n", x); //1
	/**
	 * z = 2, y = 2;
	 * y == z  => true
	 * x = true = 1
	 */

	x = (y & z); printf("x = %d\n", x); //2
	/**
	 * z = 2, y = 2;
	 * y & z => 0010 => 2
	 * x = 2
	 */

	x = (y && z); printf("x = %d\n", x); //1
	/**
	 * z = 2, y=2;
	 * y && z => 1
	 * x = 1;
	 */


	y = 4;

	x = (y | z); printf("x = %d\n", x); //6
	/**
	 * z = 2, y = 4;
	 * y | z
	 * 0010
	 * 0100
	 * 0110  => 6
	 * x = 6;
	 */

	x = (y || z); printf("x = %d\n", x); //1
	/**
	 * z = 2, y=4;
	 * y || z => true => 1
	 * x = 1;
	 */



	x = (y == z) ? 4 : 5;
	/**
	 * z = 2, y = 4;
	 * y == z => false => 0
	 * 0 ? 4 : 5 => 5
	 * x = 5;
	 */

	printf("x = %d\n", x); //5



	x = (y == z) ? 1 : (y < z) ? 2 : 3;
	/**
	 * z = 2, y = 4,
	 * 0 ? 1:(y<z)?2:3;
	 * (y<z)?2:3
	 * (4<2) => false => 0
	 * 0?2:3 => 3
	 * x = 3
	 */

	printf("x = %d\n", x); //3



	return 0;

}
