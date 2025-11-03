#include <bits/stdc++.h>
using namespace std;

struct Room {
    string name;
    int capacity;
};

struct Student {
    string name;
    vector<string> courses;
};

// ===== Function Declarations =====
void buildConflictGraph(const vector<string>& courses, const vector<Student>& students, vector<vector<int>>& adj);
int colorGraph(const vector<vector<int>>& adj, vector<int>& color);
void allocateAndDisplay(const vector<string>& courses, const vector<int>& color,
                        const vector<Student>& students, const vector<Room>& rooms);

// ===== Main Function =====
int main() {
//    ios::sync_with_stdio(false);
  //  cin.tie(nullptr);

    int nCourses, nStudents, nRooms;
    cout << "Enter number of courses: ";
    cin >> nCourses;

    vector<string> courses(nCourses);
    unordered_map<string, int> courseIndex;
    cout << "Enter course names:\n";
    for (int i = 0; i < nCourses; ++i) {
        cin >> courses[i];
        courseIndex[courses[i]] = i;
    }

    cout << "Enter number of students: ";
    cin >> nStudents;
    vector<Student> students(nStudents);

    for (int i = 0; i < nStudents; ++i) {
        cout << "\nEnter student name: ";
        cin >> students[i].name;

        int count;
        cout << "Enter number of courses enrolled: ";
        cin >> count;
        cout << "Enter course codes:\n";

        unordered_set<string> uniqueCourses; // avoid duplicates
        for (int j = 0; j < count; ++j) {
            string cname;
            cin >> cname;

            // Validate course name
            if (courseIndex.find(cname) == courseIndex.end()) {
                cerr << "Warning: Course " << cname << " not found! Skipping.\n";
                continue;
            }
            uniqueCourses.insert(cname);
        }

        // Copy unique and valid course list
        for (const auto& c : uniqueCourses)
            students[i].courses.push_back(c);
    }

    cout << "\nEnter number of rooms: ";
    cin >> nRooms;
    vector<Room> rooms(nRooms);

    for (int i = 0; i < nRooms; ++i) {
        cout << "Enter room name and capacity: ";
        cin >> rooms[i].name >> rooms[i].capacity;
    }

    // Step 1: Build Conflict Graph
    vector<vector<int>> adj(nCourses);
    buildConflictGraph(courses, students, adj);

    // Step 2: Graph Coloring (Time Slot Allocation)
    vector<int> color(nCourses, -1);
    int totalSlots = colorGraph(adj, color);
    cout << "\nTotal Time Slots Used: " << totalSlots << "\n";

    // Step 3: Room Allocation + Display Timetable
    allocateAndDisplay(courses, color, students, rooms);

    return 0;
}

// ===== Function Definitions =====

// Build conflict graph (edges between courses with common students)
void buildConflictGraph(const vector<string>& courses, const vector<Student>& students, vector<vector<int>>& adj) {
    unordered_map<string, int> index;
    for (int i = 0; i < courses.size(); ++i)
        index[courses[i]] = i;

    for (const auto& s : students) {
        for (size_t i = 0; i < s.courses.size(); ++i) {
            for (size_t j = i + 1; j < s.courses.size(); ++j) {
                int c1 = index[s.courses[i]];
                int c2 = index[s.courses[j]];

                // Avoid duplicates
                if (find(adj[c1].begin(), adj[c1].end(), c2) == adj[c1].end()) {
                    adj[c1].push_back(c2);
                    adj[c2].push_back(c1);
                }
            }
        }
    }
}

// Greedy Graph Coloring (descending degree heuristic)
int colorGraph(const vector<vector<int>>& adj, vector<int>& color) {
    int n = adj.size();
    vector<int> degree(n);
    for (int i = 0; i < n; ++i)
        degree[i] = adj[i].size();

    // Sort by degree descending
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return degree[a] > degree[b];
    });

    int maxColor = 0;
    for (int idx : order) {
        vector<bool> available(n, true);
        for (int v : adj[idx])
            if (color[v] != -1)
                available[color[v]] = false;

        int chosen = 0;
        while (chosen < n && !available[chosen]) ++chosen;
        color[idx] = chosen;
        maxColor = max(maxColor, chosen);
    }

    return maxColor + 1; // total colors used
}

// Allocate Rooms and Display Final Timetable
void allocateAndDisplay(const vector<string>& courses, const vector<int>& color,
                        const vector<Student>& students, const vector<Room>& rooms) {
    // Count how many students are in each course
    unordered_map<string, int> studentCount;
    for (const auto& s : students)
        for (const auto& c : s.courses)
            studentCount[c]++;

    int maxSlot = *max_element(color.begin(), color.end());
    vector<vector<pair<string, string>>> timetable(maxSlot + 1);

    for (int t = 0; t <= maxSlot; ++t) {
        // Find all exams in this slot
        vector<pair<string, int>> exams;
        for (int i = 0; i < courses.size(); ++i)
            if (color[i] == t)
                exams.push_back({courses[i], studentCount[courses[i]]});

        // Sort exams and rooms
        sort(exams.begin(), exams.end(), [](auto& a, auto& b) {
            return a.second > b.second;
        });

        auto sortedRooms = rooms;
        sort(sortedRooms.begin(), sortedRooms.end(), [](auto& a, auto& b) {
            return a.capacity > b.capacity;
        });

        vector<bool> used(sortedRooms.size(), false);

        for (auto& exam : exams) {
            bool assigned = false;
            for (int r = 0; r < sortedRooms.size(); ++r) {
                if (!used[r] && sortedRooms[r].capacity >= exam.second) {
                    timetable[t].push_back({exam.first, sortedRooms[r].name});
                    used[r] = true;
                    assigned = true;
                    break;
                }
            }
            if (!assigned)
                timetable[t].push_back({exam.first, "No Room Available"});
        }
    }

    // Display final timetable
    cout << "\n===== FINAL EXAM TIMETABLE =====\n";
    for (int t = 0; t <= maxSlot; ++t) {
        cout << "\nTime Slot " << t + 1 << ":\n";
        for (auto& e : timetable[t])
            cout << "  Course: " << e.first << "  →  Room: " << e.second << "\n";
    }
}

