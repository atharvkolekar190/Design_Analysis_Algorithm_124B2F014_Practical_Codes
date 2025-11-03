#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iomanip>  // for put_time
#include <sstream>

using namespace std;

struct Order {
    string order_id;
    time_t timestamp;
};

// Generate random orders
void generate_sample_orders(vector<Order> &orders, int n) {
    tm base_time = {};
    base_time.tm_year = 2025 - 1900;
    base_time.tm_mon = 5;  // June (0-based)
    base_time.tm_mday = 24;
    base_time.tm_hour = 12;

    time_t base = mktime(&base_time);

    for (int i = 0; i < n; i++) {
        int random_minutes = rand() % 100000; // up to ~70 days
        time_t order_time = base + (random_minutes * 60);

        orders.push_back({ "ORD" + to_string(i + 1), order_time });
    }
}

// Merge function for merge sort
void merge(vector<Order> &orders, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Order> L(orders.begin() + left, orders.begin() + mid + 1);
    vector<Order> R(orders.begin() + mid + 1, orders.begin() + right + 1);

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].timestamp <= R[j].timestamp)
            orders[k++] = L[i++];
        else
            orders[k++] = R[j++];
    }

    while (i < n1)
        orders[k++] = L[i++];

    while (j < n2)
        orders[k++] = R[j++];
}

void merge_sort(vector<Order> &orders, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(orders, left, mid);
        merge_sort(orders, mid + 1, right);
        merge(orders, left, mid, right);
    }
}

void print_first_n_orders(const vector<Order> &orders, int n) {
    for (int i = 0; i < n && i < (int)orders.size(); i++) {
        tm *tm_info = gmtime(&orders[i].timestamp);
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", tm_info);
        cout << "Order ID: " << orders[i].order_id << ", Timestamp: " << buffer << '\n';
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    const int NUM_ORDERS = 1000000;
    vector<Order> orders;
    orders.reserve(NUM_ORDERS);

    generate_sample_orders(orders, NUM_ORDERS);
    merge_sort(orders, 0, NUM_ORDERS - 1);

    print_first_n_orders(orders, 10);

    return 0;
}

