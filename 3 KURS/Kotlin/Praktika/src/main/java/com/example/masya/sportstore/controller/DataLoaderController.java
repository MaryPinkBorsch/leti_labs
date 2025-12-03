package com.example.masya.sportstore.controller;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.masya.sportstore.entity.Brand;
import com.example.masya.sportstore.entity.Category;
import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.entity.User;
import com.example.masya.sportstore.service.BrandService;
import com.example.masya.sportstore.service.CategoryService;
import com.example.masya.sportstore.service.ClothingService;
import com.example.masya.sportstore.service.OrderService;
import com.example.masya.sportstore.service.UserService;

@RestController
@RequestMapping("/api/test-data")
@CrossOrigin(origins = "*")
public class DataLoaderController {

    private final BrandService brandService;
    private final CategoryService categoryService;
    private final ClothingService clothingService;
    private final UserService userService;
    private final OrderService orderService;

    @Autowired
    public DataLoaderController(BrandService brandService,
            CategoryService categoryService,
            ClothingService clothingService,
            UserService userService,
            OrderService orderService) {
        this.brandService = brandService;
        this.categoryService = categoryService;
        this.clothingService = clothingService;
        this.userService = userService;
        this.orderService = orderService;
    }

    @PostMapping("/load-all")
    public ResponseEntity<String> loadAllTestData() {
        try {
            // Clear existing data (optional)
            // orderService.deleteAll();
            // clothingService.deleteAll();
            // brandService.deleteAll();
            // categoryService.deleteAll();
            // userService.deleteAll();

            // Create brands
            Brand nike = new Brand("Nike", 1964, 5);
            Brand adidas = new Brand("Adidas", 1949, 5);
            Brand puma = new Brand("Puma", 1948, 4);

            nike = brandService.save(nike);
            adidas = brandService.save(adidas);
            puma = brandService.save(puma);

            // Create categories
            Category runningShoes = new Category("Running Shoes");
            Category basketball = new Category("Basketball Apparel");
            Category football = new Category("Football Jerseys");

            runningShoes = categoryService.save(runningShoes);
            basketball = categoryService.save(basketball);
            football = categoryService.save(football);

            // Create users
            User user1 = new User("john@email.com", "password123", "John", "Doe");
            User user2 = new User("jane@email.com", "password123", "Jane", "Smith");

            user1 = userService.save(user1);
            user2 = userService.save(user2);

            // Create clothing items
            Clothing clothing1 = new Clothing();
            clothing1.setModelName("Air Max 270");
            clothing1.setSeason(2);
            clothing1.setPrice(150);
            clothing1.setBrand(nike);
            clothing1.setCategory(runningShoes);

            Clothing clothing2 = new Clothing();
            clothing2.setModelName("Ultraboost 22");
            clothing2.setSeason(2);
            clothing2.setPrice(180);
            clothing2.setBrand(adidas);
            clothing2.setCategory(runningShoes);

            Clothing clothing3 = new Clothing();
            clothing3.setModelName("Basketball Jersey");
            clothing3.setSeason(2);
            clothing3.setPrice(80);
            clothing3.setBrand(nike);
            clothing3.setCategory(basketball);

            clothingService.save(clothing1);
            clothingService.save(clothing2);
            clothingService.save(clothing3);

            return ResponseEntity.ok("Test data loaded successfully!");

        } catch (Exception e) {
            return ResponseEntity.badRequest().body("Error loading test data: " + e.getMessage());
        }
    }
}
