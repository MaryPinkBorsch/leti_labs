package com.example.masya.sportstore.service;

import com.example.masya.sportstore.entity.User;
import java.util.List;
import java.util.Optional;

public interface UserService {

    List<User> findAll();

    Optional<User> findById(Long id);

    User save(User user);

    void deleteById(Long id);

    Optional<User> findByEmail(String email);
}