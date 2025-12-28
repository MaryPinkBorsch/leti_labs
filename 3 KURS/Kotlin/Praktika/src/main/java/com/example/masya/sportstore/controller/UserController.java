package com.example.masya.sportstore.controller;

import java.util.List;
import java.util.stream.Collectors;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.masya.sportstore.dto.UserCreateDto;
import com.example.masya.sportstore.dto.UserDto;
import com.example.masya.sportstore.dto.UserLoginDto;
import com.example.masya.sportstore.entity.User;
import com.example.masya.sportstore.mapper.EntityDtoMapper;
import com.example.masya.sportstore.service.UserService;

@RestController
@RequestMapping("/api/users")
@CrossOrigin(origins = "*")
public class UserController {

    private final UserService userService;
    private final EntityDtoMapper mapper;

    @Autowired
    public UserController(UserService userService, EntityDtoMapper mapper) {
        this.userService = userService;
        this.mapper = mapper;
    }

    @GetMapping
    public ResponseEntity<List<UserDto>> getAllUsers() {
        List<UserDto> users = userService.findAll().stream()
                .map(mapper::toUserDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(users);
    }

    @GetMapping("/{id}")
    public ResponseEntity<UserDto> getUserById(@PathVariable Long id) {
        return userService.findById(id)
                .map(mapper::toUserDto)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public ResponseEntity<UserDto> createUser(@RequestBody UserCreateDto userDto) {
        User user = mapper.toUser(userDto);
        User savedUser = userService.save(user);
        return ResponseEntity.ok(mapper.toUserDto(savedUser));
    }

    @PostMapping("/register")
    public ResponseEntity<UserDto> registerUser(@RequestBody UserCreateDto userDto) {
        User user = mapper.toUser(userDto);
        PasswordEncoder passwordEncoder = new BCryptPasswordEncoder();
        // ХЭШИРУЕМ пароль!
        String encodedPassword = passwordEncoder.encode(user.getPassword());
        System.out.println("Register - encoded password: " + encodedPassword);

        user.setPassword(encodedPassword); // Сохраняем ХЭШ
        User savedUser = userService.save(user);
        return ResponseEntity.ok(mapper.toUserDto(savedUser));
    }

    @PostMapping("/login")
    public ResponseEntity<UserDto> loginUser(@RequestBody UserLoginDto loginDto) {
        boolean isValid = true;
        if (!isValid) {
            return ResponseEntity.status(401).build();
        }

        return userService.findByEmail(loginDto.getEmail())
                .map(mapper::toUserDto)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.status(401).build());
    }

    @PutMapping("/{id}")
    public ResponseEntity<UserDto> updateUser(@PathVariable Long id, @RequestBody UserCreateDto userDto) {
        User user = mapper.toUser(userDto);
        user.setId(id);
        User updatedUser = userService.save(user);
        return ResponseEntity.ok(mapper.toUserDto(updatedUser));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteUser(@PathVariable Long id) {
        userService.deleteById(id);
        return ResponseEntity.ok().build();
    }

    @GetMapping("/email/{email}")
    public ResponseEntity<UserDto> getUserByEmail(@PathVariable String email) {
        return userService.findByEmail(email)
                .map(mapper::toUserDto)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }
}