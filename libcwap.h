struct libcwap_functions {
    void (*)(time_t) time_set_function;
    void (*)(time_t) alarm_set_timestamp;
    // etc.
};

inline void libcwap_action(uint8_t (*)(uint8_t *, uint8_t));
inline void libcwap_register(struct libcwap_functions *);
