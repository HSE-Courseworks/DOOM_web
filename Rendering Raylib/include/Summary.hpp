#ifndef SUMMARY_HPP
#define SUMMARY_HPP

#include "Page.hpp"
#include "ScoreTable.hpp"

#define TIME_EXIT (5 + 1)

class Summary : Page
{
public:
    Summary();
    void show() const override;
    Pages update() override;

    void setTimeEnd(const double time);
    void setFinalScoreTable(const ScoreTable &scoreTable);

private:
    double whenEnd;
    unsigned char alpha, step;
    ScoreTable finalScore;
    Texture2D title;
    Font font;
};

#endif