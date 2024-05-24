#include <gtest/gtest.h>
#include "raylib.h"
#include "Tools.hpp"

TEST(ToolsTest, DegToRad) {
    EXPECT_NEAR(DegToRad(0), 0, 1e-5);
    EXPECT_NEAR(DegToRad(90), M_PI / 2, 1e-5);
    EXPECT_NEAR(DegToRad(180), M_PI, 1e-5);
    EXPECT_NEAR(DegToRad(360), 2 * M_PI, 1e-5);
}

TEST(ToolsTest, RadToDeg) {
    EXPECT_NEAR(RadToDeg(0), 0, 1e-5);
    EXPECT_NEAR(RadToDeg(M_PI / 2), 90, 1e-5);
    EXPECT_NEAR(RadToDeg(M_PI), 180, 1e-5);
    EXPECT_NEAR(RadToDeg(2 * M_PI), 360, 1e-5);
}

TEST(ToolsTest, GetDist2Points) {
    Vector2 v1 = {0, 0};
    Vector2 v2 = {3, 4};
    EXPECT_NEAR(getDist2Points(v1, v2), 5, 1e-5);

    v1 = {1, 1};
    v2 = {4, 5};
    EXPECT_NEAR(getDist2Points(v1, v2), 5, 1e-5);

    v1 = {0, 0};
    v2 = {0, 0};
    EXPECT_NEAR(getDist2Points(v1, v2), 0, 1e-5);
}

TEST(ToolsTest, ChangeLightness) {
    Color color = {100, 100, 100, 255};
    Color newColor = changeLightness(color, 20);
    EXPECT_EQ(newColor.r, 125); 
    EXPECT_EQ(newColor.g, 125);
    EXPECT_EQ(newColor.b, 125);
    EXPECT_EQ(newColor.a, 255);

    color = {250, 250, 250, 255};
    newColor = changeLightness(color, 20);
    EXPECT_EQ(newColor.r, 255); 
    EXPECT_EQ(newColor.g, 255);
    EXPECT_EQ(newColor.b, 255);
    EXPECT_EQ(newColor.a, 255);
}

TEST(ToolsTest, ConstrainAngle360) {
    EXPECT_NEAR(constrainAngle360(0), 0, 1e-5);
    EXPECT_NEAR(constrainAngle360(360), 0, 1e-5);
    EXPECT_NEAR(constrainAngle360(720), 0, 1e-5);
    EXPECT_NEAR(constrainAngle360(-360), 0, 1e-5);
    EXPECT_NEAR(constrainAngle360(450), 90, 1e-5);
    EXPECT_NEAR(constrainAngle360(-90), 270, 1e-5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
