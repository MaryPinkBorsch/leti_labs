package com.example.masya.sportstore.service.impl;

import com.example.masya.sportstore.entity.Clothing;
import com.example.masya.sportstore.entity.User;
import com.example.masya.sportstore.repository.ClothingRepository;
import com.example.masya.sportstore.repository.UserRepository;
import com.example.masya.sportstore.service.ShoppingCartService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

@Service
public class ShoppingCartServiceImpl implements ShoppingCartService {

    // In-memory cart storage (in production, consider using Redis or database)
    private final Map<Long, Map<Long, Integer>> userCarts = new ConcurrentHashMap<>();
    
    private final UserRepository userRepository;
    private final ClothingRepository clothingRepository;

    @Autowired
    public ShoppingCartServiceImpl(UserRepository userRepository, 
                                 ClothingRepository clothingRepository) {
        this.userRepository = userRepository;
        this.clothingRepository = clothingRepository;
    }

    @Override
    public void addToCart(Long userId, Long clothingId, Integer quantity) {
        validateUserAndClothing(userId, clothingId);
        
        Map<Long, Integer> userCart = userCarts.computeIfAbsent(userId, k -> new HashMap<>());
        userCart.merge(clothingId, quantity, Integer::sum);
    }

    @Override
    public void removeFromCart(Long userId, Long clothingId) {
        validateUserAndClothing(userId, clothingId);
        
        Map<Long, Integer> userCart = userCarts.get(userId);
        if (userCart != null) {
            userCart.remove(clothingId);
            // Remove user entry if cart is empty
            if (userCart.isEmpty()) {
                userCarts.remove(userId);
            }
        }
    }

    @Override
    public void updateCartItemQuantity(Long userId, Long clothingId, Integer quantity) {
        validateUserAndClothing(userId, clothingId);
        
        if (quantity <= 0) {
            removeFromCart(userId, clothingId);
            return;
        }
        
        Map<Long, Integer> userCart = userCarts.computeIfAbsent(userId, k -> new HashMap<>());
        userCart.put(clothingId, quantity);
    }

    @Override
    public void clearCart(Long userId) {
        validateUser(userId);
        userCarts.remove(userId);
    }

    @Override
    public Map<Clothing, Integer> getCartItems(Long userId) {
        validateUser(userId);
        
        Map<Long, Integer> itemQuantities = userCarts.getOrDefault(userId, new HashMap<>());
        Map<Clothing, Integer> cartItems = new HashMap<>();
        
        for (Map.Entry<Long, Integer> entry : itemQuantities.entrySet()) {
            Clothing clothing = clothingRepository.findById(entry.getKey())
                .orElseThrow(() -> new RuntimeException("Clothing not found with id: " + entry.getKey()));
            cartItems.put(clothing, entry.getValue());
        }
        
        return cartItems;
    }

    @Override
    public Integer getCartItemsCount(Long userId) {
        validateUser(userId);
        
        Map<Long, Integer> userCart = userCarts.get(userId);
        if (userCart == null) {
            return 0;
        }
        
        return userCart.values().stream().mapToInt(Integer::intValue).sum();
    }

    @Override
    public Double calculateCartTotal(Long userId) {
        Map<Clothing, Integer> cartItems = getCartItems(userId);
        
        return cartItems.entrySet().stream()
            .mapToDouble(entry -> {
                Clothing clothing = entry.getKey();
                Integer quantity = entry.getValue();
                return (clothing.getPrice() != null ? clothing.getPrice().doubleValue() : 0.0) * quantity;
            })
            .sum();
    }

    @Override
    public void mergeCarts(Long temporaryUserId, Long permanentUserId) {
        validateUser(temporaryUserId);
        validateUser(permanentUserId);
        
        Map<Long, Integer> tempCart = userCarts.get(temporaryUserId);
        if (tempCart != null) {
            Map<Long, Integer> permanentCart = userCarts.computeIfAbsent(permanentUserId, k -> new HashMap<>());
            
            for (Map.Entry<Long, Integer> entry : tempCart.entrySet()) {
                permanentCart.merge(entry.getKey(), entry.getValue(), Integer::sum);
            }
            
            userCarts.remove(temporaryUserId);
        }
    }

    @Override
    public boolean isItemInCart(Long userId, Long clothingId) {
        validateUserAndClothing(userId, clothingId);
        
        Map<Long, Integer> userCart = userCarts.get(userId);
        return userCart != null && userCart.containsKey(clothingId);
    }

    private void validateUser(Long userId) {
        if (!userRepository.existsById(userId)) {
            throw new RuntimeException("User not found with id: " + userId);
        }
    }

    private void validateUserAndClothing(Long userId, Long clothingId) {
        validateUser(userId);
        if (!clothingRepository.existsById(clothingId)) {
            throw new RuntimeException("Clothing not found with id: " + clothingId);
        }
    }
}