package com.example.masya.sportstore.mapper;

import com.example.masya.sportstore.dto.*;
import com.example.masya.sportstore.entity.*;
import org.springframework.stereotype.Component;

import java.util.stream.Collectors;

@Component
public class EntityDtoMapper {

    // Brand mappings
    public BrandDto toBrandDto(Brand brand) {
        if (brand == null) return null;
        
        BrandDto dto = new BrandDto();
        dto.setId(brand.getId());
        dto.setName(brand.getName());
        dto.setSince(brand.getSince());
        dto.setRating(brand.getRating());
        
        if (brand.getClothes() != null) {
            dto.setClothes(brand.getClothes().stream()
                    .map(this::toClothingDto)
                    .collect(Collectors.toList()));
        }
        
        return dto;
    }

    public Brand toBrand(BrandCreateDto dto) {
        if (dto == null) return null;
        
        Brand brand = new Brand();
        brand.setName(dto.getName());
        brand.setSince(dto.getSince());
        brand.setRating(dto.getRating());
        return brand;
    }

    // Category mappings
    public CategoryDto toCategoryDto(Category category) {
        if (category == null) return null;
        
        CategoryDto dto = new CategoryDto();
        dto.setId(category.getId());
        dto.setCategoryName(category.getCategoryName());
        
        if (category.getClothes() != null) {
            dto.setClothes(category.getClothes().stream()
                    .map(this::toClothingDto)
                    .collect(Collectors.toList()));
        }
        
        return dto;
    }

    public Category toCategory(CategoryCreateDto dto) {
        if (dto == null) return null;
        
        Category category = new Category();
        category.setCategoryName(dto.getCategoryName());
        return category;
    }

    // Clothing mappings
    public ClothingDto toClothingDto(Clothing clothing) {
        if (clothing == null) return null;
        
        ClothingDto dto = new ClothingDto();
        dto.setId(clothing.getId());
        dto.setModelName(clothing.getModelName());
        dto.setSeason(clothing.getSeason());
        dto.setPrice(clothing.getPrice());
        dto.setBrand(toBrandDto(clothing.getBrand()));
        dto.setCategory(toCategoryDto(clothing.getCategory()));
        return dto;
    }

    public Clothing toClothing(ClothingCreateDto dto, Brand brand, Category category) {
        if (dto == null) return null;
        
        Clothing clothing = new Clothing();
        clothing.setModelName(dto.getModelName());
        clothing.setSeason(dto.getSeason());
        clothing.setPrice(dto.getPrice());
        clothing.setBrand(brand);
        clothing.setCategory(category);
        return clothing;
    }

    // User mappings
    public UserDto toUserDto(User user) {
        if (user == null) return null;
        
        UserDto dto = new UserDto();
        dto.setId(user.getId());
        dto.setEmail(user.getEmail());
        dto.setFirstName(user.getFirstName());
        dto.setLastName(user.getLastName());
        dto.setCreatedAt(user.getCreatedAt());
        
        if (user.getOrders() != null) {
            dto.setOrders(user.getOrders().stream()
                    .map(this::toOrderDto)
                    .collect(Collectors.toList()));
        }
        
        return dto;
    }

    public User toUser(UserCreateDto dto) {
        if (dto == null) return null;
        
        User user = new User();
        user.setEmail(dto.getEmail());
        user.setPassword(dto.getPassword());
        user.setFirstName(dto.getFirstName());
        user.setLastName(dto.getLastName());
        return user;
    }

    // Order mappings
    public OrderDto toOrderDto(Order order) {
        if (order == null) return null;
        
        OrderDto dto = new OrderDto();
        dto.setId(order.getId());
        dto.setOrderDate(order.getOrderDate());
        dto.setStatus(order.getStatus());
        dto.setUser(toUserDto(order.getUser()));
        
        if (order.getOrderedClothes() != null) {
            dto.setOrderedClothes(order.getOrderedClothes().stream()
                    .map(this::toClothingDto)
                    .collect(Collectors.toList()));
        }
        
        // Calculate total amount
        if (order.getOrderedClothes() != null) {
            double total = order.getOrderedClothes().stream()
                    .mapToDouble(clothing -> clothing.getPrice() != null ? clothing.getPrice().doubleValue() : 0.0)
                    .sum();
            dto.setTotalAmount(total);
        }
        
        return dto;
    }
}