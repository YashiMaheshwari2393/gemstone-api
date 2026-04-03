#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <string>
using namespace std;
using json = nlohmann::json;
 
// Class to handle numerology logic
class Numerology {
private:
    int day;
public:
    Numerology(int d) {
        day = d;
    }
    int calculateMulank() {
        int sum = 0;
        int temp = day;
        while (temp > 0) {
            sum += temp % 10;
            temp /= 10;
        }
        while (sum > 9) {
            int tempSum = 0;
            while (sum > 0) {
                tempSum += sum % 10;
                sum /= 10;
            }
            sum = tempSum;
        }
        return sum;
    }
};
 
// Class for gemstone recommendation
class GemstoneRecommender {
public:
    json recommend(int mulank) {
        json result;
        result["mulank"] = mulank;
        switch (mulank) {
            case 1:
                result["gemstone"] = "Ruby";
                result["benefit"]  = "Leadership, confidence, and success";
                break;
            case 2:
                result["gemstone"] = "Pearl";
                result["benefit"]  = "Emotional balance and calmness";
                break;
            case 3:
                result["gemstone"] = "Yellow Sapphire";
                result["benefit"]  = "Wisdom and prosperity";
                break;
            case 4:
                result["gemstone"] = "Hessonite (Gomed)";
                result["benefit"]  = "Stability and protection";
                break;
            case 5:
                result["gemstone"] = "Emerald";
                result["benefit"]  = "Communication and intelligence";
                break;
            case 6:
                result["gemstone"] = "Diamond";
                result["benefit"]  = "Luxury, love, and charm";
                break;
            case 7:
                result["gemstone"] = "Cat's Eye";
                result["benefit"]  = "Spiritual growth and intuition";
                break;
            case 8:
                result["gemstone"] = "Blue Sapphire";
                result["benefit"]  = "Discipline and power";
                break;
            case 9:
                result["gemstone"] = "Red Coral";
                result["benefit"]  = "Energy and courage";
                break;
            default:
                result["gemstone"] = "Unknown";
                result["benefit"]  = "Invalid Mulank";
        }
        result["note"] = "Based on numerology insights (not guaranteed results)";
        return result;
    }
};
 
int main() {
    httplib::Server svr;
 
    // Health check
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(
            "Numerology Gemstone API is running!\n"
            "Endpoints:\n"
            "  GET  /                -> Health check\n"
            "  GET  /gemstone        -> Usage hint\n"
            "  POST /gemstone        -> Get gemstone recommendation\n"
            "  Body: { \"day\": 15, \"month\": 8, \"year\": 2000 }\n",
            "text/plain");
    });
 
    // GET /gemstone - usage hint
    svr.Get("/gemstone", [](const httplib::Request&, httplib::Response& res) {
        json hint;
        hint["usage"]   = "Send a POST request to /gemstone";
        hint["example"] = {{"day", 15}, {"month", 8}, {"year", 2000}};
        res.set_content(hint.dump(2), "application/json");
    });
 
    // POST /gemstone - main endpoint
    svr.Post("/gemstone", [](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            json e;
            e["error"] = "Invalid JSON body";
            res.status = 400;
            res.set_content(e.dump(), "application/json");
            return;
        }
 
        if (!body.contains("day") || !body.contains("month") || !body.contains("year")) {
            json e;
            e["error"] = "Missing fields. Required: day, month, year";
            res.status = 400;
            res.set_content(e.dump(), "application/json");
            return;
        }
 
        int day   = body["day"];
        int month = body["month"];
        int year  = body["year"];
 
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
            json e;
            e["error"] = "Invalid date values.";
            res.status = 400;
            res.set_content(e.dump(), "application/json");
            return;
        }
 
        Numerology num(day);
        int mulank = num.calculateMulank();
 
        GemstoneRecommender recommender;
        json result = recommender.recommend(mulank);
 
        result["date_of_birth"] = to_string(day) + "-" + to_string(month) + "-" + to_string(year);
 
        res.set_content(result.dump(2), "application/json");
    });
 
    const char* port_env = getenv("PORT");
    int port = port_env ? atoi(port_env) : 8080;
 
    cout << "Numerology Gemstone API running on port " << port << endl;
    svr.listen("0.0.0.0", port);
    return 0;
}
 
