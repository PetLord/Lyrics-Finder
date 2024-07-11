#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_LyricsFinderUI.h"
#include "BoyerMore.H"
#include "LyricsFinder.H"
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <vector>

class LyricsFinder;

class LyricsFinderUI : public QMainWindow
{
    Q_OBJECT

public:
    LyricsFinderUI(QWidget* parent = nullptr);
    ~LyricsFinderUI();

private:
    Ui::LyricsFinderUIClass ui;
    string keys;
    int mode;
    LyricsFinder* lFinder;
    QPushButton* searchButton;
    QTextEdit* searchBox;
    QLabel* title;
    QLabel* subTitle;
    vector<QPushButton*> songHits;
    QCheckBox* checkExactMatch;
    vector<pair<int, pair<string, string>>> hits;
    

    const QString backgroundStyle = "background-color: #150606;";
    const QString searchBoxStyle = "background-color: #f2f2f2; border: none; color: black; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; border-radius: 10px;";
    const QString titleStyle = "color: #f5f5f5; font-weight: bold;";
    const QString subTitleStyle = "color: black; font-size: 20px; font-weight: bold;";
    const QString songButtonStyle =
        "QPushButton {\
        background-color: #401A1A; \
        color: white;\
        border-radius: 10px;\
        padding-left: 10px; \
		padding-right: 10px; \
        border: black 1px; \
        text-align: center; \
        text-decoration: none; \
        display: inline-block; \
        font-size: 20px;\
        }\
        QPushButton:hover{ \
        cursor: pointer; \
        border-width: 2px; \
        background-color: #5A3535; \
        }\
        QPushButton : pressed{ \
        background-color: #5A4545; \
        }";

    const QString searchButtonStyle = 
        "QPushButton {\
        background-color: #401A1A; \
        border-radius: 10px;\
        color: white;\
        border: none;\
        padding: 15px 32px;\
        text-align: center;\
        text-decoration: none;\
        display: inline-block;\
        font-size: 20px;\
        }\
		QPushButton:hover {\
        cursor: pointer; \
        border-width: 2px; \
		background-color: #5A3535;\
		}\
        QPushButton:pressed {\
		background-color: #5A4545;\
		}\
        ";
    const QString lyricsPageTitleStyle = "color: #f5f5f5; font-size: 20px; font-weight: bold; text-align: center;";
    const QString lyricsPageLyricsStyle = "color: #f5f5f5; font-size: 15px; text-align: center;";
    const QString lyricsPageSubTitleStyle = "color: #f5f5f5; font-size: 20px; text-align: center;";
    const QString lyricsPageMatchStyle = "color: #f5f5f5; background-color: #228B22 ;font-size: 20px; text-align: center;";
    const QString lyricsPageWrongStyle = "color: #f5f5f5; background-color: #3d0405 ;font-size: 20px; text-align: center;";

    void setFrontPage();
    void setSearchButton();
    void setSearchBox();
    void setTitle();
    void setSubTitle();
    void setCheckBox();
    void setSongButtons();
    void searchLyrics();
    void openSong(string artist, string song);
    void wrongPressed(QWidget* wdg, string artist, string song);
    void matchPressed(QWidget* wdg);

public slots:
    void resizeEvent(QResizeEvent*);
};
