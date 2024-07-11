#include "LyricsFinderUI.h"
#include "BoyerMore.H"
#include "LyricsFinder.H"
#include <vector>
#include <fstream>
#include <QResizeEvent>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QRegularExpression>

LyricsFinderUI::LyricsFinderUI(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    
    string path = "songs";
    this->lFinder = new LyricsFinder(path);

    this->searchButton = new QPushButton("Search", this);
    this->searchBox = new QTextEdit(this);
    this->title = new QLabel("Lyrics Finder", this);
    this->subTitle = new QLabel("Search music lyrics by keywords", this);
    this->checkExactMatch = new QCheckBox("Exact Match", this);
    this->setMinimumHeight(300);
    this->setMinimumWidth(500);
    this->showMaximized();
    this->setStyleSheet(backgroundStyle);
    connect(searchButton, &QPushButton::released, this, [this]() {searchLyrics(); });

    setFrontPage();
}

void LyricsFinderUI::resizeEvent(QResizeEvent* event)
{
    int x = event->size().width();
    int y = event->size().height();
    setFrontPage();
}

void LyricsFinderUI::setFrontPage()
{
	setTitle();
	setSubTitle();
	setSearchBox();
	setSearchButton();
    setCheckBox();
    setSongButtons();
}

void LyricsFinderUI::setTitle()
{
    QFont font = title->font();
    font.setPointSize(20 + (this->height() + min(this->width(),1000)) / 50);
    title->setFont(font);

    int w = title->sizeHint().width();
    int h = title->sizeHint().height();
	title->setGeometry(this->width()*0.5 - w / 2, this->height()*0.1, w, h);
    title->setStyleSheet(titleStyle);
    title->show();
}

void LyricsFinderUI::setSubTitle()
{
    QFont font = subTitle->font();
    font.setPointSize(5 + (this->height() + min(this->width(), 1000)) / 155);
    subTitle->setFont(font);

    int w = subTitle->sizeHint().width();
    int h = subTitle->sizeHint().height();
    subTitle->setGeometry(this->width() * 0.5 - w / 2,  title->geometry().bottom()+10, w, h);
}

void LyricsFinderUI::setSearchButton()
{
    QFont font = searchButton->font();
    font.setPointSize(7 + (this->height() + min(this->width(), 1000)) / 155);
    searchButton->setFont(font);

    int w = searchButton->sizeHint().width();
    int h = searchButton->sizeHint().height();
    searchButton->setGeometry(searchBox->geometry().right()+7, subTitle->geometry().bottom() + this->width() * 0.05, w, h);
    searchButton->setStyleSheet(searchButtonStyle);

    searchButton->show();
}

void LyricsFinderUI::setSearchBox()
{
    QFont font = searchBox->font();
    font.setPointSize(7 + (this->height() + min(this->width(), 1000)) / 155);
    searchBox->setFont(font);

    int w = this->width() * 0.4;
    int h = searchButton->sizeHint().height();
    searchBox->setGeometry(this->width() * 0.5250 -  w /(1.5), subTitle->geometry().bottom() + this->width() * 0.05, w, h);
    searchBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    searchBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    searchBox->setStyleSheet(searchBoxStyle);
    searchBox->show();
}

void LyricsFinderUI::setCheckBox()
{
    int w = checkExactMatch->sizeHint().width();
    int h = checkExactMatch->sizeHint().height();
	checkExactMatch->setGeometry(searchBox->geometry().left(), searchBox->geometry().bottom() + 10, w, h);
	checkExactMatch->show();
}

void LyricsFinderUI::setSongButtons()
{
    // clear the existing song buttons
    for (int i = 0; i < songHits.size(); i++)
    {
        delete songHits[i];
    }
    songHits.clear();

    int baseHeight = checkExactMatch->geometry().bottom() + this->width() * 0.01;
    int changingHeight = this->height() * 0.1;

    for (int i = 0; i < min((int)hits.size(),5); i++)
    {
        QPushButton* songButton = new QPushButton(QString::fromUtf8(hits[i].second.first + ": " + hits[i].second.second + ": " + to_string(hits[i].first) + " occurences"), this);
        connect(songButton, &QPushButton::released, this, [this, i]() {openSong(hits[i].second.first, hits[i].second.second); });
        QFont font = songButton->font();
        songButton->setStyleSheet(songButtonStyle);
        
        int w = this->width() * 0.3;
        int minw = songButton->sizeHint().width();
        songButton->setGeometry(min(this->width() * 0.5 - w / 2, this->width() * 0.5 - minw / 2), baseHeight + (i * changingHeight), max((int)(this->width() * 0.3), minw), max(changingHeight-5, 45));
        songButton->setStyleSheet(songButtonStyle);
        songButton->show();
        songHits.push_back(songButton);
    }
}

void LyricsFinderUI::searchLyrics()
{
    mode = checkExactMatch->isChecked();

    for (int i = 0; i < songHits.size(); i++)
    {
        delete songHits[i];
    }
    songHits.clear(); // remove all existing song buttons
    hits.clear(); // remove all existing matches

    string text = searchBox->toPlainText().toStdString();
    searchBox->setText("");

    keys = text;
    lFinder->findLyrics(text, mode);

    hits = lFinder->getTopHits(-1);
    setSongButtons();
}

void LyricsFinderUI::wrongPressed(QWidget* wdg, string artist, string song)
{
    wdg->close();
    if (!hits.empty())
    {
        for (int i = 0; i < hits.size(); i++)
        {
            if (hits[i].second.first == artist && hits[i].second.second == song)
			{
				hits.erase(hits.begin() + i);
				break;
			}   
        }

        if (hits.empty())
        {
            QMessageBox::information(this, "Sorry", "Sorry we were unable to find your song");
        }
        setSongButtons();
    }
}

void LyricsFinderUI::matchPressed(QWidget* wdg)
{
    wdg->close();
    QMessageBox::information(this, "Success", "Song Matched!");
    hits.clear();
    setSongButtons();
}

void LyricsFinderUI::openSong(string artist, string song)
{
    QWidget* wdg = new QWidget;
    wdg->setStyleSheet(backgroundStyle);
    wdg->setWindowTitle("");
    wdg->setFixedSize(1100, 800);

    QVBoxLayout* layout = new QVBoxLayout(wdg);
    QScrollArea* scrollArea = new QScrollArea(wdg);

    QLabel* title = new QLabel(QString::fromUtf8(artist + ": " + song + ": "), wdg);
    title->setStyleSheet(lyricsPageTitleStyle);
    int titleWidth = title->sizeHint().width();
    int titleHeight = title->sizeHint().height();
    title->setGeometry(wdg->width()*0.5 - titleWidth /2, 50, titleWidth, titleHeight);
    title->setAlignment(Qt::AlignCenter);

    QLabel* lyrics = new QLabel(wdg);
    QString lyricsText = QString::fromUtf8(lFinder->getLyrics(artist, song));
    QString p = QString::fromUtf8(keys);
    QString pattern;

    if (mode)
    {
        pattern = QString("\\b%1\\b").arg(QRegularExpression::escape(p));
    }
    else
    {
		pattern = QString("%1").arg(QRegularExpression::escape(p));
    }
     
    QString newpattern = QString::fromUtf8("<span style='color: red;'>" + keys + "</span>");
    QRegularExpression regex(pattern);
    QString styledText = lyricsText.replace(regex, newpattern);
    styledText.replace("\n", "<br>");
    lyrics->setText(styledText);
    lyrics->setWordWrap(true);
    lyrics->setStyleSheet(lyricsPageLyricsStyle);
    lyrics->setTextFormat(Qt::RichText);

    scrollArea->setWidget(lyrics);
    scrollArea->setWidgetResizable(true);

    int buttonWidth = wdg->width() * 0.1;
    int buttonHeight = wdg->height() * 0.05;

    QPushButton* matchButton = new QPushButton("Yes", wdg);
    matchButton->setStyleSheet(lyricsPageMatchStyle);
    matchButton->setGeometry(wdg->width()*0.85, wdg->height() * 0.9, buttonWidth, buttonHeight);

    QPushButton* wrongButton = new QPushButton("No", wdg);
    wrongButton->setStyleSheet(lyricsPageWrongStyle);
    wrongButton->setGeometry(wdg->width() * 0.725, wdg->height() * 0.9, buttonWidth, buttonHeight);

    QLabel* subTitle = new QLabel("Is this the lyrics you want to find?", wdg);
    subTitle->setStyleSheet(lyricsPageSubTitleStyle);
    subTitle->setAlignment(Qt::AlignCenter);

    connect(wrongButton, &QPushButton::released, this, [this, wdg, artist, song]() {wrongPressed(wdg, artist, song); });
    connect(matchButton, &QPushButton::released, this, [this, wdg]() {matchPressed(wdg); });
    
    layout->addWidget(title);
    layout->addWidget(scrollArea);
    layout->addWidget(subTitle);
    layout->addWidget(matchButton);
    layout->addWidget(wrongButton);
    wdg->setLayout(layout);
    wdg->show();
}

LyricsFinderUI::~LyricsFinderUI()
{
    delete lFinder;
    delete searchButton;
    delete searchBox;
    delete title;
    delete subTitle;
    delete checkExactMatch;
    for (int i = 0; i < songHits.size(); i++)
    {
        delete songHits[i];
    }
    
}

