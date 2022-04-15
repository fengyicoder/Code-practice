#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include "graphic.h"

struct LineToPointAdapter
{
    using Points = std::vector<Point>;
    LineToPointAdapter(Line& line) {
        static boost::hash<Line> hash;
        line_hash = hash(line);
        if (cache.find(line_hash) != cache.end()) return;
        Points points;
        int left = min(line.start.x, line.end.x);
        int right = max(line.start.x, line.end.x);
        int top = min(line.start.y, line.end.y);
        int bottom = max(line.start.y, line.end.y);
        int dx = right - left;
        int dy = line.end.y - line.start.y;

        if (dx == 0) {
            for (int y=top; y<=bottom; ++y)
            {
                points.emplace_back(Point{left, y});
            }
        }
        else if (dy == 0) {
            for (int x=left; x<=right; ++x)
            {
                points.emplace_back(Point{x, top});
            }
        }
        cache[line_hash] = points;
    }
    virtual Points::iterator begin() {
        return cache[line_hash].begin();
    }

    virtual Points::iterator end() {
        return cache[line_hash].end();
    }
private:
    // Points points;
    static std::map<size_t, Points> cache;
    std::size_t line_hash;
};
