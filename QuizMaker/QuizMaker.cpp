/*
QuizMaker.cpp

Podstawowe cele i zadania:

      Quiz Maker - make an application which takes various questions from a file, picked
  randomly, and puts together a quiz for students.Each quiz can be different and then
  reads a key to grade the quizzes.

Platforma:
    Visual C++, Windows / gcc, Linux

Autor: Mateusz Gasiorowski
Student AGH
Wydzial: WIET
Kierunek: Elektronika

Data ostatniej modyfikacji: 02.01.2024
*/



#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>  

using namespace std;


///////////////////////////////////////////////////////////
//  Metoda sprawdzająca czy podany string składa się z cyfr
///////////////////////////////////////////////////////////
bool is_digits(string& str)
{
    for (char ch : str) {
        if (!(ch >= '0' && ch <= '9')) 
        {
            return false;
        }
    }
    return true;
}


///////////////////////////////////////////////////////////
//  Klasa odpowiadająca za pojedyńcze pytanie do Quizu
///////////////////////////////////////////////////////////
class Question {
public:
    Question(const string& question, const vector<string>& answers, int correctAnswerIndex)
        : question(question), answers(answers), correctAnswerIndex(correctAnswerIndex) {}

    int displayQuestion() const
    {
        if (correctAnswerIndex > -1)
        {
            cout << "Question: " << question << endl;

            for (int i = 0; i < answers.size(); i++)
            {
                cout << "  " << i + 1 << ". " << answers[i] << endl;
            }
            cout << " Enter your answer (1-" << answers.size() << "): ";
            return (int)answers.size();
        }
        return -1;
    }

    bool checkAnswer(int userAnswer) const 
    {
        if (userAnswer - 1 == correctAnswerIndex) 
        {
            cout << "Correct!" << endl << endl;
            return true;
        }
        else 
        {
            cout << "Incorrect. The correct answer is " << correctAnswerIndex + 1 << ": " << answers[correctAnswerIndex] << endl << endl;
        }

        return false;
    }
    int getIndex()
    {
        return correctAnswerIndex;
    }

private:
    string question;
    vector<string> answers;
    int correctAnswerIndex;
};

///////////////////////////////////////////////////////////
//  Klasa odpowiadająca za funkcje obsługi Quizu
///////////////////////////////////////////////////////////
class Quiz
{
public:
    Quiz(vector<Question>& QuizQuestions)
        : QuizQuestions(QuizQuestions)  {}

    void displayNextQuestion()
    {
        QuestionNumber++;
        NumberOfAnswers = QuizQuestions[QuestionNumber].displayQuestion();
        if(NumberOfAnswers == -1)
        {
            displayNextQuestion();
        }
    }

    int isTheEnd()
    {
        if (QuestionNumber+1 >= (int)QuizQuestions.size())
        {
            return 0;
        }
        return 1;
    }

    void ValidateAnswer(int userAnswer)
    {
        if (QuizQuestions[QuestionNumber].checkAnswer(userAnswer) )
        {
            CorrectAnswers++;
        }
    }

    int getUserAnswer() 
    {
        string userInput;
        int userAnswer;

        while (1) {
            cin >> userInput;

            if (userInput.length() == 1 && isdigit(userInput[0])) 
            {
                userAnswer = userInput[0] - '0';
                if (userAnswer >= 1 && userAnswer <= NumberOfAnswers) {
                    break; 
                }
            }
            cout << "Invalid answer. Please enter a digit between 1 and " << NumberOfAnswers << ": "; 
        }
        return userAnswer;
    }

    void displayResults()
    {
        cout << endl << "Your total score is: " << CorrectAnswers << "/" << QuizQuestions.size() << endl;
        system("pause");
    }


private:
    vector<Question> QuizQuestions;
    int CorrectAnswers = 0;
    int QuestionNumber = -1;
    int NumberOfAnswers = 0;
};


///////////////////////////////////////////////////////////
//  Metoda odpowiedzialna za interfejs Quizu
///////////////////////////////////////////////////////////
void QuizConsoleStart(vector<Question>& AllQuestions)
{
    system("cls");
    Quiz QuizMaker(AllQuestions);

    while(QuizMaker.isTheEnd())
    {
        QuizMaker.displayNextQuestion();
        QuizMaker.ValidateAnswer(QuizMaker.getUserAnswer());
    } 
    QuizMaker.displayResults();
}

///////////////////////////////////////////////////////////
//  Metoda za parsowanie pliku txt z pytaniem
///////////////////////////////////////////////////////////
Question parseFile(const string& filename)
{
    string question;
    vector<string> answers;
    int correctAnswerIndex = 0;

    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cout << "Error opening the file: "  << filename << endl;
        system("pause");
        return Question("", {}, -1);
    }

    string line;
    while (getline(inputFile, line))
    {
        question = line;

        for(int i = 0 ; i < 10 ; i++)
        {
            getline(inputFile, line);
            if (line == "========")
            {
                break;
            }
            else if (i == 9 && line != "========")
            {
                cout << "Error while parsing question: " << filename << endl;
                system("pause");
                return Question("", {}, -1);
            }
            else
            {
                answers.push_back(line);
            }
        }
        getline(inputFile, line);
        if (isdigit(line[0]))
        {
            correctAnswerIndex = stoi(line);
        }
        else
        {
            cout << "Error while parsing question: " << filename << endl;
            system("pause");
            return Question("", {}, -1);
        }
    }
    inputFile.close();
    return Question(question, answers, correctAnswerIndex);;
}
///////////////////////////////////////////////////////////
//  Metoda odpowiedzialna za interfejs użytkownika
///////////////////////////////////////////////////////////
bool QuizConfigAndStart()
{
    system("cls");
    int QuizQuestionsNumber = 0;
    cout << "Enter the number of questions: ";
    string userInput;
    while (1)
    {
        cin >> userInput;
        if (is_digits(userInput))
        {
            QuizQuestionsNumber = stoi(userInput);
            break;
        }
        else
        {
            cout << endl << "Error: NAN. Please enter a number: ";
        }
    }
    int QuizQuestionsNumberInDir = 0;

    string filename = "";
    for (int i = 0; i < 1000; i++)
    {
        filename = "Questions/";
        filename.append(to_string(i));
        filename.append(".txt");
        ifstream inputFile(filename);
        if (!inputFile.is_open())
        {
            QuizQuestionsNumberInDir = i;
            break;
        }
    }
    if (QuizQuestionsNumberInDir < QuizQuestionsNumber)
    {
        cout << "Number of avilable questions: " << QuizQuestionsNumberInDir << " and it is less than required number of questions." << endl;
        cout << "Create Quiz with max avilable number od questions? (Y/N):";
        while (1)
        {
            cin >> userInput;
            if (userInput == "Y" || userInput == "y")
            {
                QuizQuestionsNumber = QuizQuestionsNumberInDir;
                cout << "New number of questions in Quiz: " << QuizQuestionsNumber << endl;
                break;
            }
            else if (userInput == "N" || userInput == "n")
            {
                cout << "Resetting..." << endl;
                return true;
            }
            else
            {
                cout << endl << "Error: Syntax Unrecognized command" << endl << "Please enter (Y/N): ";
            }
        }
    }
    else
    {
        cout << "Number of avilable questions: " << QuizQuestionsNumberInDir << endl;
    }
    vector<int> QuestionsNumberInFile = {};
    for (int i = 0; i < QuizQuestionsNumber ; i++)
    {
        int loopcounter = 0;
        bool validNumber = false;
        int numberOfQuestion = 0;
        while(!validNumber)
        {
            validNumber = true;
            numberOfQuestion = rand() % QuizQuestionsNumberInDir;
            for (int i : QuestionsNumberInFile)
            {
                if (i == numberOfQuestion)
                    validNumber = false;
            }
            if (loopcounter >= QuizQuestionsNumber * 10)
            {
                cout << "Fatal error" << endl;
                return true;
            }
            loopcounter++;
        }
        QuestionsNumberInFile.push_back(numberOfQuestion);
    }
    vector<Question> AllQuestions;
    for (int i : QuestionsNumberInFile)
    {
        filename = "Questions/";
        filename.append(to_string(i));
        filename.append(".txt");
        AllQuestions.push_back(parseFile(filename));
        if (AllQuestions[AllQuestions.size()-1].getIndex() == -1)
        {
            AllQuestions.pop_back();
        }
    }
    QuizConsoleStart(AllQuestions);
    return false;
}

int main()
{
    srand(time(NULL));
    while (QuizConfigAndStart())
    {}
    return 0;
}
