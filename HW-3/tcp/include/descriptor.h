//
// Created by Osip Chin on 25.10.2020.
//

#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

class Descriptor {
public:
    Descriptor();
    explicit Descriptor(int fd);
    ~Descriptor();

    Descriptor(const Descriptor&) = delete;
    Descriptor& operator=(const Descriptor&) = delete;

    Descriptor(Descriptor&& tmp) noexcept;
    Descriptor& operator=(Descriptor&& tmp) noexcept;

    void close();

    void set_fd(int fd);
    int get_fd() const;
private:
    int fd_;
};



#endif //DESCRIPTOR_H
