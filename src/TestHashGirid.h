#pragma once
#include <gtest/gtest.h>
class TestHashGirid:public testing::Test
{
public:
	TestHashGirid()
	{
		// initialization code here
	}

	~TestHashGirid()
	{
		// cleanup any pending stuff, but no exceptions allowed
	}

	void SetUp() override
	{
		// code here will execute just before the test ensues
	}

	void TearDown() override
	{
		// code here will be called just after the test completes
		// ok to through exceptions from here if need be
	}	
};

TEST_F(TestHashGirid, TestHashGrid)
{
	// Test code here
}