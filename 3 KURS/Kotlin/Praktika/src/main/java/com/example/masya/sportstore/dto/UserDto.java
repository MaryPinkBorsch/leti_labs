package com.example.masya.sportstore.dto;

import java.time.LocalDateTime;
import java.util.List;

public class UserDto extends BaseDto {
    private String email;
    private String firstName;
    private String lastName;
    private LocalDateTime createdAt;
    private List<OrderDto> orders;

    // Constructors
    public UserDto() {}

    public UserDto(String email, String firstName, String lastName) {
        this.email = email;
        this.firstName = firstName;
        this.lastName = lastName;
    }

    // Getters and Setters
    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }
    public String getFirstName() { return firstName; }
    public void setFirstName(String firstName) { this.firstName = firstName; }
    public String getLastName() { return lastName; }
    public void setLastName(String lastName) { this.lastName = lastName; }
    public LocalDateTime getCreatedAt() { return createdAt; }
    public void setCreatedAt(LocalDateTime createdAt) { this.createdAt = createdAt; }
    public List<OrderDto> getOrders() { return orders; }
    public void setOrders(List<OrderDto> orders) { this.orders = orders; }
}

class UserCreateDto {
    private String email;
    private String password;
    private String firstName;
    private String lastName;

    // Getters and Setters
    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }
    public String getPassword() { return password; }
    public void setPassword(String password) { this.password = password; }
    public String getFirstName() { return firstName; }
    public void setFirstName(String firstName) { this.firstName = firstName; }
    public String getLastName() { return lastName; }
    public void setLastName(String lastName) { this.lastName = lastName; }
}

class UserLoginDto {
    private String email;
    private String password;

    // Getters and Setters
    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }
    public String getPassword() { return password; }
    public void setPassword(String password) { this.password = password; }
}