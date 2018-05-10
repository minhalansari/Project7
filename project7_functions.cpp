//project 07 
//9 functions to connect and disconnet users from servername
//first function AddConnection adds given user to the given server returns true if done successfull and returns false if user is already connected
//second function DeleteConnection removes user from given server returns true if done successfully and returns false if user is already not in the server
//third function ParseServerData opens and reads a given file and goes line by line and finds the leave or join command and performs the task throws errors if can't find the join or leave command or if the file is not opened properly
//fourth function printall prints all the elements in the map 
//fifth function AllServers returns a set of strings of all the server names in sd
//sixth function AllUsers returns a set of strings of all the usernames in sd
//seventh function HasConnections returns a set of strings of all the server names that the given username is connected to
//eighth function HasUsers returns a set of strings of all the users in given server
//ninth function BalanceServers gets the unique users in each server and divdes them evenly among the two servers given

#include<iostream>
using std::cout; using std::cin; using std::endl; using std::boolalpha;
#include "proj07_functions.h"
#include <string>
using std::string;
#include <utility>     
using UserName = const std::string &;using std::pair; using std::make_pair;
#include<set>
using std::set;
#include <stdexcept>      
using std::invalid_argument; using std::domain_error;
#include <fstream>
using std::ifstream;
#include<vector>
using std::vector;
#include <algorithm>
using std::set_symmetric_difference;
#include <sstream>
using std::istringstream;
#include<iterator>
using std::ostream_iterator;




bool AddConnection(ServerData &sd, ServerName sn,const std::string & un){
    auto search = sd.find(sn);
    std::set<std::string> set1;
    if(search != sd.end()){ //if user is already connected
        set1 = sd[sn];
        if (set1.find(un) != set1.end()){
            return false;
        }
        else{
            sd[sn].insert(un);
            return true;  
        }
        
    }
    else{
        sd[sn].insert(un);
        return true;
        
    }
}

bool DeleteConnection(ServerData &sd, ServerName sn, UserName un){
    auto search = sd.find(sn);
    std::set<std::string> set1;
    if(search != sd.end() ){ //if user needs to be deleted
        set1= sd[sn];
        if (set1.find(un) != set1.end()){
            sd[sn].erase(un);
            return true;
        }
        else{ //user not connected
           return false; 
        }
        
        
    }
    else{ //user not connected
        return false;
    }
}

ServerData ParseServerData(const std::string &fname){
    ServerData sd;
    ifstream inFile;
    inFile.open(fname); //open file
    if(inFile.fail()){ //throw error if file failed to open
        throw std::invalid_argument("Cannot be opened.");
    }
    string line;
    vector<string> filedata;
    while(!inFile.eof()){ //read file and separate lines into words
        std::getline (inFile,line);
        
        
        istringstream iss(line);
        while (iss) {
            string word;
            iss >> word;
            if(word != ""){
                
                filedata.push_back(word);
            }
            
        }
    }
    
   
    int count = 0;
    for(int i =0; i< filedata.size(); i++){
        count +=1;
        
        if(count == 3){ //check every third word for join or leave
            count = 0;
            if(filedata[i-1] == "join" or filedata[i-1] == "leave"){
              
                if(filedata[i-1] == "join"){
                    
                   sd[filedata[i]].insert(filedata[i-2]);
                }
                if(filedata[i-1]== "leave"){
                   
                   sd[filedata[i]].erase(filedata[i-2]);
                }
           }
            
            else{ //throw error if not join or leave
             
                throw std::domain_error("wrong command.");
            }
            
            
        }
    }
    
    return sd;
    

}



void PrintAll(std::ostream &out, const ServerData &sd){
    
    for (const auto& pair : sd){ //itereate through map and print
        out << pair.first << " :";
        for (const auto& s : pair.second){
            out << " " << s;
        }
        out<<"\n";
        
    }

}
std::set<std::string> AllServers(const ServerData &sd){
    set<string> final;
    for(const auto& x : sd){ //iterate through keys in map and add to set
        final.insert(x.first);
    }
    return final;
}
std::set<std::string> AllUsers(const ServerData &sd){
    set<string> final1;
    for(const auto& v : sd){ //iterate through keys then users and add users to set
        string key = v.first;
        for(const auto& p : v.second){
            final1.insert(p);
        }
    }
    return final1;
}

std::set<std::string> HasConnections(const ServerData &sd, UserName un){
    set<string> servernames; //iterate through keys and then users add key to set if the value contains the username
    for(const auto& n : sd){
        string key = n.first;
        for(const auto& m : n.second){
            if(m == un){
                servernames.insert(key);
            }
        }
    }
    return servernames;
    
    
}

std::set<std::string> HasUsers(const ServerData &sd, ServerName sn){
    set<string> usernames;
    for(const auto& f : sd){ //iterate through keys and then through values if the key matches the given server name and add usernames to set
        string key = f.first;
        if(key == sn){
            for(const auto& t : f.second){
                usernames.insert(t);
            }
        }
    }
    return usernames;
}
void BalanceServers(ServerData &sd, ServerName sn1, ServerName sn2){
    
    set<string> result;
    set<string> sn1_users = HasUsers(sd, sn1);
    set<string> sn2_users = HasUsers(sd, sn2);
    set_symmetric_difference(sn1_users.begin(), sn1_users.end(),sn2_users.begin(), sn2_users.end(), inserter(result, result.begin()));
     for (set<string>::const_iterator i = result.begin(); i != result.end(); ++i) {
      
        DeleteConnection(sd, sn1, *i);
        DeleteConnection(sd, sn2, *i); //delete unique values from map
    }
    if(result.size() % 2 == 0){ //add half to the first server and second half to second server
        int halfnum = result.size()/2;
        int count = 0;
        for (set<string>::const_iterator i = result.begin(); i != result.end(); ++i) {
            if(count < halfnum){
                sd[sn1].insert(*i);
                count += 1;
            }
            else{
                count +=1;
                sd[sn2].insert(*i);
            }
                
        }
        
    }
    else{
        int sn1_num = result.size() /2 +1; //add half plus one to first server if odd number of unique values
        int count = 0;
        for (set<string>::const_iterator i = result.begin(); i != result.end(); ++i) {
            if(count < sn1_num){
                sd[sn1].insert(*i);
                count += 1;
            }
            else{
                count +=1;
                sd[sn2].insert(*i);
            }
                
        }
    }
}

