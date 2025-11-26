package com.example.masya.sportstore.mapper;

import java.util.stream.Collectors;

import org.springframework.stereotype.Component;

import com.example.masya.sportstore.dto.BrandCreateDto;
import com.example.masya.sportstore.dto.BrandDto;
import com.example.masya.sportstore.dto.CategoryCreateDto;
import com.example.masya.sportstore.dto.CategoryDto;
import com.example.masya.sportstore.dto.ClothingCreateDto;
import com.example.masya.sportstore.dto.ClothingDto;
import com.example.masya.sportstore.dto.OrderDto;
import com.example.masya.sportstore.dto.UserCreateDto;
import com.example.masya.sportstore.dto.UserDto;
import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.entity.Category;
import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.entity.Order;
import com.example.masya.sportstore.entity.User;

@Component
public class EntityDtoMapper {

    // User mappings
    public UserDto toUserDto(User user) {
        if (user == null) {
            return null;
        }

        UserDto dto = new UserDto();
        dto.setId(user.getId());
        dto.setEmail(user.getEmail());
        dto.setFirstName(user.getFirstName());
        dto.setLastName(user.getLastName());
        dto.setCreatedAt(user.getCreatedAt());
        // Note: Don't map password for security reasons
        return dto;
    }

    public User toUser(UserDto userDto) {
        if (userDto == null) {
            return null;
        }

        User user = new User();
        user.setId(userDto.getId());
        user.setEmail(userDto.getEmail());
        user.setFirstName(userDto.getFirstName());
        user.setLastName(userDto.getLastName());
        user.setCreatedAt(userDto.getCreatedAt());
        return user;
    }

    public User toUser(UserCreateDto createUserDto) {
        if (createUserDto == null) {
            return null;
        }

        User user = new User();
        user.setEmail(createUserDto.getEmail());
        user.setPassword(createUserDto.getPassword()); // Should be encoded
        user.setFirstName(createUserDto.getFirstName());
        user.setLastName(createUserDto.getLastName());
        return user;
    }

    // Clothing mappings
    public ClothingDto toClothingDto(Clothing clothing) {
        if (clothing == null) {
            return null;
        }

        ClothingDto dto = new ClothingDto();
        dto.setId(clothing.getId());
        dto.setModelName(clothing.getModelName());
        dto.setSeason(clothing.getSeason());
        dto.setPrice(clothing.getPrice());

        return dto;
    }

    public Clothing toClothing(ClothingDto clothingDto, Brand brand, Category category) {
        if (clothingDto == null) {
            return null;
        }

        Clothing clothing = new Clothing();
        clothing.setId(clothingDto.getId());
        clothing.setModelName(clothingDto.getModelName());
        clothing.setSeason(clothingDto.getSeason());
        clothing.setPrice(clothingDto.getPrice());
        clothing.setBrand(brand);
        clothing.setCategory(category);
        return clothing;
    }

    public Clothing toClothing(ClothingCreateDto createClothingDto, Brand brand, Category category) {
        if (createClothingDto == null) {
            return null;
        }

        Clothing clothing = new Clothing();
        clothing.setModelName(createClothingDto.getModelName());
        clothing.setSeason(createClothingDto.getSeason());
        clothing.setPrice(createClothingDto.getPrice());
        clothing.setBrand(brand);
        clothing.setCategory(category);
        return clothing;
    }

    // Brand mappings
    public BrandDto toBrandDto(Brand brand) {
        if (brand == null) {
            return null;
        }

        BrandDto dto = new BrandDto();
        dto.setId(brand.getId());
        dto.setName(brand.getName());
        dto.setSince(brand.getSince());
        dto.setRating(brand.getRating());
        return dto;
    }

    public Brand toBrand(BrandDto brandDto) {
        if (brandDto == null) {
            return null;
        }

        Brand brand = new Brand();
        brand.setId(brandDto.getId());
        brand.setName(brandDto.getName());
        brand.setSince(brandDto.getSince());
        brand.setRating(brandDto.getRating());
        return brand;
    }

    public Brand toBrand(BrandCreateDto brandCreateDto) {
        if (brandCreateDto == null) {
            return null;
        }

        Brand brand = new Brand();
        brand.setName(brandCreateDto.getName());
        brand.setSince(brandCreateDto.getSince());
        brand.setRating(brandCreateDto.getRating());
        return brand;
    }


    // Category mappings
    public CategoryDto toCategoryDto(Category category) {
        if (category == null) {
            return null;
        }

        CategoryDto dto = new CategoryDto();
        dto.setId(category.getId());
        dto.setCategoryName(category.getCategoryName());
        return dto;
    }

    public Category toCategory(CategoryDto categoryDto) {
        if (categoryDto == null) {
            return null;
        }

        Category category = new Category();
        category.setId(categoryDto.getId());
        category.setCategoryName(categoryDto.getCategoryName());
        return category;
    }

    public Category toCategory(CategoryCreateDto categoryCreateDto) {
        if (categoryCreateDto == null) {
            return null;
        }

        Category category = new Category();
        category.setCategoryName(categoryCreateDto.getCategoryName());
        return category;
    }

    // Order mappings
    public OrderDto toOrderDto(Order order) {
        if (order == null) {
            return null;
        }

        OrderDto dto = new OrderDto();
        dto.setId(order.getId());
        dto.setOrderDate(order.getOrderDate());
        dto.setStatus(order.getStatus());

        if (order.getOrderedClothes() != null) {
            dto.setOrderedClothes(order.getOrderedClothes().stream()
                    .map(this::toClothingDto)
                    .collect(Collectors.toList()));
        }

        return dto;
    }

}
