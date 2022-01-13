#include <list>
#include <set>
#include <unordered_map>

using namespace std;

static int timestamp;

class Tweet {
public:
    int tweetId;
    int timestamp;
    Tweet *next;
public:
    Tweet(int id, int time): tweetId{id}, timestamp(time), next(nullptr) {}
};

class User {
private:
    int userId;
    set<int> follower;
    Tweet *head;
public:
    User(int id): userId{id}, head{nullptr} {
        follow(id);
    }

    void follow(int id) {
        follower.insert(id);
    }

    void unfollow(int id) {
        if (id != userId) follower.erase(id);
    }

    void post(int tweetId) {
        Tweet *tweet = new Tweet(tweetId, timestamp);
        timestamp++;
        tweet->next = head;
        head = tweet;
    }
};

class Twitter {
public:
    void postTweet(int userId, int tweetId) {
        if (!userMap.count(userId)) {
            userMap.insert(make_pair(userId, new User(userId)));
        }
        User user = userMap[userId];
        user.post(tweetId);
    }

    list<int> getNewsFeed(int userId) {

    }

    void follow(int followerId, int followeeId) {
        if (!userMap.count(followerId)) {
            User *user = new User(followerId);
            userMap.insert(make_pair(followerId, user));
        }
        if (!userMap.count(followeeId)) {
            User *user = new User(followeeId);
            userMap.insert(make_pair(followeeId, user));
        }
        userMap[followerId].follow(followeeId);
    }

    void unfollow(int followerId, int followeeId) {

    }
private:
    // static int timestamp;
    static User user;
    static Tweet tweet;
    static unordered_map<int, User> userMap;
};