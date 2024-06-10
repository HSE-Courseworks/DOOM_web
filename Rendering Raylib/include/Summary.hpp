#ifndef SUMMARY_HPP
#define SUMMARY_HPP

#include "Page.hpp"
#include "ScoreTable.hpp"

#define TIME_EXIT (5 + 1)

class Summary : Page {
public:
    Summary();
    void show() const override;
    Pages update() override;
    void setTimeEnd(double time);
    void setFinalScoreTable(const ScoreTable& scoreTable);

private:
    Font font;
    double whenEnd;
    unsigned char alpha, step;
    ScoreTable finalScore;
    Texture2D title;
};

#endif