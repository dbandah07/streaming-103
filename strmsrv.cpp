#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

// To do - Complete this function
StreamService::StreamService()

{
    // Update as needed
    cUser_ = nullptr; // 
}

// To do - Complete this function
StreamService::~StreamService()
{
  for (size_t i = 0; i < users_.size(); i++) {
    delete users_[i];
  }

  for (size_t i = 0; i < content_.size(); i++) {
    delete content_[i];
  }
}

// Complete - Do not alter
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
    p.parse(is, content_, users_);
    cout << "Read " << content_.size() << " content items." << endl;
    cout << "Read " << users_.size() << " users." << endl;
}

// To do - Complete this function
void StreamService::userLogin(const std::string& uname)
{
  if (cUser_ != nullptr) {
    throw std::runtime_error("Other user logged in.");
  }

  int idx = getUserIndexByName(uname);
  if (idx == -1) {
    throw std::invalid_argument("Invalid username");
  }

  cUser_ = users_[idx];
}

// To do - Complete this function
void StreamService::userLogout()
{
  cUser_ = nullptr;
}

// To do - Complete this function
std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
    std::vector<CID_T> results;
    for(size_t i = 0; i < content_.size(); i++){
        // TO DO - modify this to also push back when the string 'partial'
        //  is contained in the name of the current content. Lookup the string
        //  class documentation to find an appropriate function that can be used
        //  to do this simply.
        std:: string cname = content_[i]->name();
        if(partial == "*" || cname.find(partial) != std::string::npos){
            results.push_back(i);
        }        
    }
    return results;
}

// Complete - Do not alter
std::vector<CID_T> StreamService::getUserHistory() const
{
    throwIfNoCurrentUser();
    return cUser_->history;
}

// To do - Complete this function
void StreamService::watch(CID_T contentID)
{
  throwIfNoCurrentUser();

  if(!isValidContentID(contentID)) {
    throw std::range_error("Watch: invalid contentID");
  }

  Content* show = content_[contentID];

  if (show->rating() > cUser_->ratingLimit) {
    throw RatingLimitError("Watch: rating too high for this user");
  }
 
  cUser_->history.push_back(contentID);
  show->addViewer(cUser_->uname);

}

// To do - Complete this function
void StreamService::reviewShow(CID_T contentID, int numStars)
{
  throwIfNoCurrentUser();

  if (!isValidContentID(contentID)) {
    throw ReviewRangeError("Review: invalid contentID");
  }

  if (numStars < 0 || numStars > 5) {
    throw ReviewRangeError("Review: stars out of range (0-5)");
  }

  content_[contentID]->review(numStars);
}

// To do - Complete this function
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
    throwIfNoCurrentUser();

    if (!(isValidContentID(contentID))) {
      throw std::range_error("Invalid Content ID");
    }
    // Change this when you are ready, but for now it is a 
    // dummy implementation
    std::vector<User*> sameViewers;
    // loop through all users
    for (size_t i = 0; i < users_.size(); i++) {
      for (size_t k = 0; k < users_[i]->history.size(); k++) {
        if (users_[i]->history[k] == contentID) {
          // if the user watched the specific contentID, theyre added to the vector
          sameViewers.push_back(users_[i]);
          break;
        }
      }
    }

    std::vector<int> ids;
    std::vector<int> cnts;

    for (size_t i = 0; i < content_.size(); i++) {
      CID_T c = i; 

      if (c == contentID) {
        continue;
      }

      bool alrdySeen = false;
      for (size_t d = 0; d < cUser_->history.size(); d++) {
        if (cUser_->history[d] == c) {
          alrdySeen = true; 
          break;
        }
      }

      if (alrdySeen) {
        continue;
      }


      int numCnts = 0;
      for (size_t k = 0; k < sameViewers.size(); k++) {
        for (size_t j = 0; j < sameViewers[k]->history.size(); j++) {
          if (sameViewers[k]->history[j] == c) {
            numCnts++;
            break; // only count user once;
          }
        }
      }

      ids.push_back(c);
      cnts.push_back(numCnts);
    }

    CID_T best = -1;
    int bestCnt = 0;

    for (size_t i = 0; i < ids.size(); i++) {
      if (cnts[i] > bestCnt) {
        bestCnt = cnts[i];
        best = ids[i];
      }
    }

    return best;
}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
    // Do not alter this
    if(! isValidContentID(contentID)){
        throw std::invalid_argument("Watch: invalid contentID");
    }

    // Call the display abitlity of the appropriate content object
    content_[contentID]->display(std::cout);
}

// Complete - Do not alter
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

// Complete - Do not alter
void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

// Complete - Do not alter
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
