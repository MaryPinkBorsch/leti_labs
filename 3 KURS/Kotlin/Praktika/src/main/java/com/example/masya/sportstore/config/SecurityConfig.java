package com.example.masya.sportstore.config;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.annotation.authentication.configuration.AuthenticationConfiguration;
import org.springframework.security.config.annotation.method.configuration.EnableMethodSecurity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.web.SecurityFilterChain;

import com.example.masya.sportstore.service.CustomUserDetailsService;

@Configuration
@EnableWebSecurity
@EnableMethodSecurity
public class SecurityConfig {

    @Autowired
    private CustomUserDetailsService userDetailsService;

    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    @Bean
    public AuthenticationManager authenticationManager(AuthenticationConfiguration config) throws Exception {
        return config.getAuthenticationManager();
    }

    @Bean
    public SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
        http
            // Отключаем CSRF для API (или настраиваем для веб-форм)
            .csrf(csrf -> csrf.disable())
            
            // Настройка авторизации запросов
            .authorizeHttpRequests(authz -> authz
                // Публичные эндпоинты (доступны всем)
                .requestMatchers("/api/auth/**").permitAll()
                .requestMatchers("/api/test-data/**").permitAll()
                .requestMatchers("/h2-console/**").permitAll()
                .requestMatchers("/error").permitAll()
                
                // Административные эндпоинты
                .requestMatchers("/api/admin/**").hasRole("ADMIN")
                
                // Защищенные эндпоинты для пользователей
                .requestMatchers("/api/users/**").hasAnyRole("USER", "ADMIN")
                .requestMatchers("/api/orders/**").hasAnyRole("USER", "ADMIN")
                .requestMatchers("/api/clothing/**").permitAll() // Каталог доступен всем
                .requestMatchers("/api/brands/**").permitAll()
                .requestMatchers("/api/categories/**").permitAll()
                .requestMatchers("/api/search/**").permitAll()
                
                // Все остальные API-запросы требуют аутентификации
                .requestMatchers("/api/**").hasRole("USER")
                
                // Статические ресурсы
                .requestMatchers("/css/**", "/js/**", "/images/**", "/webjars/**").permitAll()
                
                // Все остальные запросы
                .anyRequest().authenticated()
            )
            
            // Настройка управления сессиями
            .sessionManagement(session -> session
                .sessionCreationPolicy(SessionCreationPolicy.IF_REQUIRED)
            )
            
            // Подключаем наш сервис для загрузки пользователей
            .userDetailsService(userDetailsService);
        
        return http.build();
    }
}