namespace Ansys {

enum Status {
    OK = 0,
};

class Ansys {
    public:
        Status Boot(void (*boot_task)(void *), void *input);
};

} // namespace Ansys
