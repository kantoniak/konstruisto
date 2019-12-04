#include "ShaderManager.hpp"

namespace opengl {

std::optional<Shader> ShaderManager::compileShader(Shader::ShaderType shader_type, std::string filename,
                                                   engine::Logger& log) {
  Shader shader = Shader::create(shader_type);

  if (shader.get_id() == 0) {
    log.error("Shader creating failed.");
    return std::nullopt;
  }

  std::ifstream shaderFile(filename);
  if (!shaderFile.good()) {
    log.error("Could not open/read file \"%s\".", filename.c_str());
    return std::nullopt;
  }

  std::string source_string((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
  if (!shader.compile(source_string)) {
    log.error("Shader compilation failed for \"%s\": %s", filename.c_str(), shader.get_info_log().data());
    shader.delete_shader();
    return std::nullopt;
  }

  log.debug("Compiled %s \"%s\".", to_string(shader_type).c_str(), filename.c_str());
  return shader;
}

template <size_t n>
std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, n>& shaders, engine::Logger& log) {
  ShaderProgram program;
  program.generate();

  if (program.get_id() == 0) {
    log.error("Shader program creating failed.");
    return std::nullopt;
  }

  for (auto shader : shaders) {
    program.attach(shader);
  }

  if (!program.link()) {
    log.error("Shader program linking failed: %s", program.get_info_log().data());
    program.delete_program();
    return std::nullopt;
  }

  read_uniform_blocks(program);

  log.debug("Compiled shader program.");
  return program;
}

const UniformBlock& ShaderManager::get_uniform_block(std::string name) const noexcept {
  return uniform_blocks.at(name);
}

void ShaderManager::read_uniform_blocks(const ShaderProgram& shader_program) noexcept {
  int32_t uniform_block_count;
  glGetProgramInterfaceiv(shader_program.get_id(), GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &uniform_block_count);

  const uint32_t uniform_block_props = {GL_NUM_ACTIVE_VARIABLES};
  const uint32_t uniform_block_active_vars = {GL_ACTIVE_VARIABLES};
  const std::array<uint32_t, 3> uniform_var_prop = {GL_TYPE, GL_NAME_LENGTH, GL_OFFSET};

  for (int block = 0; block < uniform_block_count; block++) {
    uniform_blocks_iterator uniform_block;

    int32_t active_vars_count;
    glGetProgramResourceiv(shader_program.get_id(), GL_UNIFORM_BLOCK, block, 1, &uniform_block_props, 1, nullptr,
                           &active_vars_count);

    if (!active_vars_count) {
      continue;
    }

    std::vector<int32_t> uniform_var_indices(active_vars_count);
    glGetProgramResourceiv(shader_program.get_id(), GL_UNIFORM_BLOCK, block, 1, &uniform_block_active_vars,
                           active_vars_count, nullptr, uniform_var_indices.data());

    for (int uniform = 0; uniform < active_vars_count; uniform++) {
      std::array<int32_t, 3> values;
      glGetProgramResourceiv(shader_program.get_id(), GL_UNIFORM, uniform_var_indices[uniform], 3,
                             uniform_var_prop.data(), 3, nullptr, values.data());

      std::string name(values[1], 0);
      glGetProgramResourceName(shader_program.get_id(), GL_UNIFORM, uniform_var_indices[uniform], name.size(), nullptr,
                               name.data());
      name.pop_back();

      if (uniform == 0) {
        // Add new uniform block if it didn't exist yet
        std::string block_name = get_uniform_block_name(name);
        uniform_block = uniform_blocks.find(block_name);
        if (uniform_block == uniform_blocks.end()) {
          uniform_block =
              uniform_blocks.emplace(block_name, UniformBlock(block_name, uniform_buffer_next_binding_block++)).first;
        }

        // Bind program on first variable
        (*uniform_block).second.bind_program(shader_program, block);
      }

      UniformBlock& ub = (*uniform_block).second;
      ub.register_uniform(BlockUniform(Type(values[0]), name, values[2]));
    }
  }
}

inline std::string ShaderManager::get_uniform_block_name(const std::string& uniform_name) const noexcept {
  return uniform_name.substr(0, uniform_name.find("."));
}

// As of OpenGL 4.6 there can be up to 5 shaders in use.
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 1>& shaders,
                                                                 engine::Logger& log);
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 2>& shaders,
                                                                 engine::Logger& log);
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 3>& shaders,
                                                                 engine::Logger& log);
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 4>& shaders,
                                                                 engine::Logger& log);
template std::optional<ShaderProgram> ShaderManager::linkProgram(const std::array<Shader, 5>& shaders,
                                                                 engine::Logger& log);
}
