/*
 * Main.cpp
 *
 *  Created on: 22.02.2012
 *      Author: Felix Wohlfrom
 */
#include <iostream>
#include "gtest/gtest.h"

TEST(Main, Basic) {
	EXPECT_TRUE(true);
}

TEST(Main, Extended) {
	EXPECT_EQ(1.0, 1.0);
}
