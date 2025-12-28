package com.example.masya.sportstore.controller;

import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.masya.sportstore.dto.LoginRequest;
import com.example.masya.sportstore.entity.User;
import com.example.masya.sportstore.repository.UserRepository;

import jakarta.validation.Valid;

@RestController
@RequestMapping("/api/auth")
public class AuthController {

    @Autowired
    private AuthenticationManager authenticationManager;

    @Autowired
    private UserRepository userRepository;

    @PostMapping("/login")
    public ResponseEntity<?> login(@Valid @RequestBody LoginRequest loginRequest) {
        try {
            // Аутентификация пользователя
            Authentication authentication = authenticationManager.authenticate(
                    new UsernamePasswordAuthenticationToken(
                            loginRequest.getEmail(),
                            loginRequest.getPassword()));

            // Установка аутентификации в контекст
            SecurityContextHolder.getContext().setAuthentication(authentication);

            // Получение информации о пользователе
            Optional<User> user = userRepository.findByEmail(loginRequest.getEmail());

            if (user.isPresent()) {
                Map<String, Object> response = new HashMap<>();
                response.put("message", "Успешный вход в систему");
                response.put("userId", user.get().getId());
                response.put("email", user.get().getEmail());
                response.put("firstName", user.get().getFirstName());
                response.put("lastName", user.get().getLastName());
                // response.put("role", user.get().getRole());
                return ResponseEntity.ok(response);
            } else {
                return ResponseEntity.badRequest().body("Пользователь не найден");
            }
        } catch (AuthenticationException e) {
            return ResponseEntity.badRequest().body("Неверный email или пароль");
        }
    }

    @PostMapping("/logout")
    public ResponseEntity<?> logout() {
        SecurityContextHolder.clearContext();
        Map<String, String> response = new HashMap<>();
        response.put("message", "Успешный выход из системы");
        return ResponseEntity.ok(response);
    }

    // Дополнительный эндпоинт для проверки текущего пользователя
    @GetMapping("/me")
    public ResponseEntity<?> getCurrentUser() {
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();

        if (authentication == null || !authentication.isAuthenticated()) {
            return ResponseEntity.status(401).body("Пользователь не аутентифицирован");
        }

        String email = authentication.getName();
        Optional<User> user = userRepository.findByEmail(email);

        if (user.isPresent()) {
            Map<String, Object> response = new HashMap<>();
            response.put("userId", user.get().getId());
            response.put("email", user.get().getEmail());
            response.put("firstName", user.get().getFirstName());
            response.put("lastName", user.get().getLastName());
            // response.put("role", user.get().getRole());
            return ResponseEntity.ok(response);
        }

        return ResponseEntity.status(404).body("Пользователь не найден");
    }

    @PostMapping("/create-test-user")
    public ResponseEntity<?> createTestUser() {
        try {
            System.out.println("=== Creating test user ===");

            // Удаляем старого пользователя, если есть
            if (userRepository.findByEmail("test@example.com").isPresent()) {

                System.out.println("Old user deleted");
            }

            // Создаем тестового пользователя
            User user = new User();
            user.setEmail("test@example.com");

            // ХЭШИРУЕМ пароль!
            String rawPassword = "password123";
            PasswordEncoder passwordEncoder = new BCryptPasswordEncoder();
           
            String encodedPassword = passwordEncoder.encode(rawPassword);
            System.out.println("Register - encoded password: " + encodedPassword);

            user.setPassword(encodedPassword); // Сохраняем ХЭШ
            System.out.println("Raw password: " + rawPassword);
            System.out.println("Encoded password: " + encodedPassword);

            user.setPassword(encodedPassword); // Сохраняем ХЭШ
            user.setFirstName("Test");
            user.setLastName("User");
            // user.setRole("USER");

            User savedUser = userRepository.save(user);
            System.out.println("User saved with ID: " + savedUser.getId());
            System.out.println("Stored password in DB: " + savedUser.getPassword());

            // Проверяем, что пароль можно проверить
            boolean matches = passwordEncoder.matches(rawPassword, savedUser.getPassword());
            System.out.println("Password verification test: " + matches);

            return ResponseEntity.ok("Test user created: test@example.com / password123");

        } catch (Exception e) {
            e.printStackTrace();
            return ResponseEntity.badRequest().body("Error: " + e.getMessage());
        }
    }
}