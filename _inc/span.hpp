#include "__preprocessor__.h"
#include <span>

void simple_array()
{
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::span<int> sp(arr.data(), arr.size() - 2);

    for (auto& elem : sp)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // Modify the array through the span
    for (auto& elem : sp)
    {
        elem = elem * elem;
    }

    for (const auto& elem : arr)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

struct AaSysComMsg
{
    int id;
    size_t payload_size;
    void* payload_ptr;

public:
    AaSysComMsg(int _id, size_t _payload_size) : id(_id), payload_size(_payload_size) { payload_ptr = malloc(payload_size); }

    ~AaSysComMsg()
    {
        if (payload_ptr)
        {
            free(payload_ptr);
            payload_ptr = nullptr;
        }
    }

    void* get_payload() { return payload_ptr; }
};

struct AaSysCom
{
    static void* msg_create(span<u8>& payload, int id, size_t payload_size)
    {
        // można też tutaj dać malloc, wypełnić ręcznie i zrobić drugi alloc
        // ale od tego jest C++, żeby to zrobił automatycznie

        AaSysComMsg* msg = new AaSysComMsg(id, payload_size);
        payload = span<u8>(static_cast<u8*>(msg->get_payload()), payload_size);

        return msg;
    }
    static void msg_send(void* msg)
    {
        AaSysComMsg* sys_msg = reinterpret_cast<AaSysComMsg*>(msg);

        cout << "Sending message with ID: " << sys_msg->id << "\n";
        cout << "Sending message with ID: " << sys_msg->payload_size << "\n";

        delete msg;
    }
};

struct DiscoveryMsg
{
    static int id;
    static size_t size;
    static void* create(span<u8>& msg_payload) { return AaSysCom::msg_create(msg_payload, id, size); }

public:
    int a;
    float b;
};

int DiscoveryMsg::id = 0x1234;
size_t DiscoveryMsg::size = sizeof(DiscoveryMsg);

void dynamic_memory()
{
    span<u8> msg_payload;
    void* msg = DiscoveryMsg::create(msg_payload);

    auto& discoveryMsg = *(reinterpret_cast<DiscoveryMsg*>(msg_payload.data()));
    discoveryMsg.a = 42;
    discoveryMsg.b = 3.14f;

    AaSysCom::msg_send(msg);
}

void span_play()
{
    var(sizeof(span<u8>));
    var(sizeof(span<u16>));
    var(sizeof(span<u32>));
    var(sizeof(span<u64>));
    dynamic_memory();
}