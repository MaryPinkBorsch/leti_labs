package com.example.masya.sportstore.controller;

import com.example.masya.sportstore.dto.CategoryCreateDto;
import com.example.masya.sportstore.dto.CategoryDto;
import com.example.masya.sportstore.entity.Category;
import com.example.masya.sportstore.mapper.EntityDtoMapper;
import com.example.masya.sportstore.service.CategoryService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/api/categories")
@CrossOrigin(origins = "*")
public class CategoryController {

    private final CategoryService categoryService;
    private final EntityDtoMapper mapper;

    @Autowired
    public CategoryController(CategoryService categoryService, EntityDtoMapper mapper) {
        this.categoryService = categoryService;
        this.mapper = mapper;
    }

    @GetMapping
    public ResponseEntity<List<CategoryDto>> getAllCategories() {
        List<CategoryDto> categories = categoryService.findAll().stream()
                .map(mapper::toCategoryDto)
                .collect(Collectors.toList());
        return ResponseEntity.ok(categories);
    }

    @GetMapping("/{id}")
    public ResponseEntity<CategoryDto> getCategoryById(@PathVariable Long id) {
        return categoryService.findById(id)
                .map(mapper::toCategoryDto)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }

    @PostMapping
    public ResponseEntity<CategoryDto> createCategory(@RequestBody CategoryCreateDto categoryDto) {
        Category category = mapper.toCategory(categoryDto);
        Category savedCategory = categoryService.save(category);
        return ResponseEntity.ok(mapper.toCategoryDto(savedCategory));
    }

    @PutMapping("/{id}")
    public ResponseEntity<CategoryDto> updateCategory(@PathVariable Long id, @RequestBody CategoryCreateDto categoryDto) {
        Category category = mapper.toCategory(categoryDto);
        category.setId(id);
        Category updatedCategory = categoryService.update(category);
        return ResponseEntity.ok(mapper.toCategoryDto(updatedCategory));
    }

    @DeleteMapping("/{id}")
    public ResponseEntity<Void> deleteCategory(@PathVariable Long id) {
        categoryService.deleteById(id);
        return ResponseEntity.ok().build();
    }

    @GetMapping("/search")
    public ResponseEntity<CategoryDto> getCategoryByName(@RequestParam String name) {
        return categoryService.findByCategoryName(name)
                .map(mapper::toCategoryDto)
                .map(ResponseEntity::ok)
                .orElse(ResponseEntity.notFound().build());
    }
}