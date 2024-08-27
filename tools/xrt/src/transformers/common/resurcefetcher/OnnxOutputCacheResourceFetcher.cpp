//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <transformers/common/resourcefetcher/OnnxOutputCacheResourceFetcher.hpp>
#include <common/Utils.hpp>

std::string_view OnnxOutputCacheResourceFetcher::name() const {
  return myName;
}

//-------------------------------------------------------------------------------------
void OnnxOutputCacheResourceFetcher::fetchResource(const ResourceIdentifier& _resourceId, const std::filesystem::path& _targetPath) {
  std::filesystem::path _basePath = getPath(ResourceDirectory::OnnxOutputCacheTmpDir);
  std::filesystem::path _sourcePath = _basePath / _resourceId.toString();

  if (!std::filesystem::exists(_sourcePath)) {
      throw ResourceNotFoundException("Not in ONNX output cache");
  }

  
  if (std::filesystem::exists(_targetPath) && !std::filesystem::is_symlink(_targetPath)) {
      throw std::runtime_error(
          fmt::format("Want to create symlink but file at {} but file exists", _targetPath.string()));
  }

  std::filesystem::remove(_targetPath);
  std::filesystem::create_symlink(_sourcePath, _targetPath);
}

//-------------------------------------------------------------------------------------
