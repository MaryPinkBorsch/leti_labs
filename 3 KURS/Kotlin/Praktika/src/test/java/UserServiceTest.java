
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import org.mockito.junit.jupiter.MockitoExtension;

import com.example.masya.sportstore.entity.User;
import com.example.masya.sportstore.repository.UserRepository;
import com.example.masya.sportstore.service.impl.UserServiceImpl;

@ExtendWith(MockitoExtension.class)
class UserServiceTest {

    @Mock
    private UserRepository userRepository;

    @InjectMocks
    private UserServiceImpl userService;

    @Test
    void testFindAllUsers() {
        // 1. ПОДГОТОВКА
        User user1 = new User("john@example.com", "password123", "John", "Doe");
        user1.setId(1L);

        User user2 = new User("jane@example.com", "password456", "Jane", "Smith");
        user2.setId(2L);

        // 2. НАСТРОЙКА
        List<User> expectedUsers = Arrays.asList(user1, user2);
        when(userRepository.findAll()).thenReturn(expectedUsers);

        // 3. ВЫПОЛНЕНИЕ
        List<User> actualUsers = userService.findAll();

        // 4. ПРОВЕРКА
        assertEquals(2, actualUsers.size());
        assertEquals("john@example.com", actualUsers.get(0).getEmail());
        assertEquals("John", actualUsers.get(0).getFirstName());
        assertEquals("jane@example.com", actualUsers.get(1).getEmail());
        assertEquals("Jane", actualUsers.get(1).getFirstName());
        verify(userRepository, times(1)).findAll();

        System.out.println("✅ Тест 1 пройден: findAll() возвращает всех пользователей");
    }

    @Test
    void testFindUserByEmail() {
        // 1. ПОДГОТОВКА
        User testUser = new User("alex@example.com", "password789", "Alex", "Johnson");
        testUser.setId(3L);

        // 2. НАСТРОЙКА
        when(userRepository.findByEmail("alex@example.com"))
                .thenReturn(java.util.Optional.of(testUser));

        // 3. ВЫПОЛНЕНИЕ
        java.util.Optional<User> foundUser = userService.findByEmail("alex@example.com");

        // 4. ПРОВЕРКА
        assertTrue(foundUser.isPresent());
        User actualUser = foundUser.get();
        assertEquals("alex@example.com", actualUser.getEmail());
        assertEquals("Alex", actualUser.getFirstName());
        assertEquals("Johnson", actualUser.getLastName());
        verify(userRepository, times(1)).findByEmail("alex@example.com");

        System.out.println("✅ Тест 2 пройден: findByEmail() находит пользователя по email");
    }

    @Test
    void testSaveUser() {
        // 1. ПОДГОТОВКА
        User newUser = new User();
        newUser.setEmail("new@example.com");
        newUser.setPassword("newpassword");
        newUser.setFirstName("New");
        newUser.setLastName("User");

        User savedUser = new User("new@example.com", "newpassword", "New", "User");
        savedUser.setId(10L);

        // 2. НАСТРОЙКА
        when(userRepository.save(newUser)).thenReturn(savedUser);

        // 3. ВЫПОЛНЕНИЕ
        User result = userService.save(newUser);

        // 4. ПРОВЕРКА
        assertEquals(10L, result.getId());
        assertEquals("new@example.com", result.getEmail());
        assertEquals("New", result.getFirstName());
        assertEquals("User", result.getLastName());
        verify(userRepository, times(1)).save(newUser);

        System.out.println("✅ Тест 3 пройден: save() сохраняет нового пользователя");
    }
}