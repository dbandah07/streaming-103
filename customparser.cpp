// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";

// To Do - Complete this function
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
    // Erase any old contents
    content.clear();
    users.clear();

    // TO DO - Add your code below.

    int n = 0; 

    if (!(is >> n)) { // if reading fails 
      throw ParserError(error_msg_1);
    }

    // cout << "DEBUG: n = " << n << endl;

    is.ignore(1000, '\n');

    // loop & read each content item
    for (int i = 0; i < n; i++) {
      // temp variables to read info in
      int id = 0; 
      int type = 0;
      int numReviews = 0;
      int totalStars = 0; 
      int rating = 0; 
      int numEp = 0;

      std::string name, line, viewers;

      // added: 
      std::string firstLine;
      getline(is >> std::ws, firstLine);
      std::stringstream first(firstLine);
      
      if (!(first >> id >> type)) {
        // cout << "DEBUG: before throw. 1" << endl;
        throw ParserError(error_msg_2);
      }


      if (!(getline(is >> std::ws, name))) {
        // cout << "DEBUG: before throw. 2" << endl;
        throw ParserError(error_msg_2);
      }


      std::string newLine;

      // numReviews'
      if (!(getline(is >> std::ws, newLine))) {
        // cout << "DEBUG: before throw. 3" << endl;
        throw ParserError(error_msg_2);
      }
      stringstream ss1(newLine);
      if (!(ss1 >> numReviews)) {
        // cout << "DEBUG: before throw. 4" << endl;
        throw ParserError(error_msg_2);
      }

      // totalStars'
      if (!(getline(is >> std::ws, newLine))) {
        // cout << "DEBUG: before throw. 5" << endl;
        throw ParserError(error_msg_2);
      }

      // cout << "DEBUG: totalStars line = [ " << newLine << " ]" << endl;
      stringstream ss2(newLine);
      if (!(ss2 >> totalStars)) {
        // cout << "DEBUG: before throw. 6" << endl;
        throw ParserError(error_msg_2);
      }

      // rating'
      if (!(getline(is >> std::ws, newLine))) {
        // cout << "DEBUG: before throw. 7" << endl;
        throw ParserError(error_msg_2);
      }

      // cout << "DEBUG: rating line = [ " << newLine << " ]" << endl;
      stringstream ss3(newLine);
      if (!(ss3 >> rating)) {
        // cout << "DEBUG: before throw. 8" << endl;
        throw ParserError(error_msg_2);
      }

      if (type == 1) { // series 
        if (!(is >> numEp)){ // read in num of Ep
          // cout << "DEBUG: before throw. 9" << endl;
          throw ParserError(error_msg_2); // throw error if fails
        }
        is.ignore(1000, '\n');
      }

      if (!(getline(is, viewers))){
        // cout << "DEBUG: before throw. 10" << endl;
        throw ParserError(error_msg_2);
      }

      Content* add = nullptr;

      if (type == 0) { // movie
        add = new Movie(id, name, numReviews, totalStars, rating);
      }

      else { // series
        add = new Series(id, name, numReviews, totalStars, rating, numEp);
      }


      // READING IN USERS
      std::stringstream theUsers(viewers);
      std::string v;

      while (theUsers >> v) { // read in users
        add->addViewer(v);
      }
      content.push_back(add);
    }

      std::string un;
      int lim = 0;
      int id = 0;

      while (is >> un) {
        if (!(is >> lim)) {
          throw ParserError(error_msg_3);
        }
        is.ignore(1000,'\n');


        User* adding = new User(un, lim);

        std::string contentIds;

        if (!(getline(is, contentIds))) {
          throw ParserError(error_msg_3);
        }

        std::stringstream cd(contentIds);

        while (cd >> id) {
          adding->history.push_back(id); // reads in content from cd
        }

        users.push_back(adding);
    }

}