#include <list>
#include <set>
#include <unordered_map>
#include <queue>

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
public:
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

struct cmp {
    bool operator() (Tweet *a, Tweet*b) {
        return a->timestamp < b->timestamp;
    }
};


class Twitter {
public:
    void postTweet(int userId, int tweetId) {
        if (!userMap.count(userId)) {
            userMap.insert(make_pair(userId, new User(userId)));
        }
        User *user = userMap[userId];
        user->post(tweetId);
    }

    list<int> getNewsFeed(int userId) {
        list<int> res;
        if (!userMap.count(userId)) return res;
        set<int> users = userMap[userId]->follower;
        priority_queue<Tweet*> pq;
        for (int id : users) {
            Tweet *twt = userMap[id]->head;
            if (!twt) continue;
            pq.push(twt);
        }
        while (!pq.empty()) {
            if (res.size() == 10) break;
            Tweet *t = pq.top();
            res.push_back(t->tweetId);
            pq.pop();
            if (t->next) pq.push(t->next);
        }
        return res;
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
        userMap[followerId]->follow(followeeId);
    }

    void unfollow(int followerId, int followeeId) {
        if (userMap.count(followerId)) {
            userMap[followerId]->unfollow(followeeId);
        }
    }
private:
    // static int timestamp;
    static User user;
    static Tweet tweet;
    static unordered_map<int, User*> userMap;
};